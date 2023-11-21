int main(void)
{
    unsigned char var1;                 //to store the pressed key
	unsigned char arr[] = "Welcome";    //String to be displayed on LCD

    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
	
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7 
                          |GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);
	
	LCD_init();                         //LCD initialization function
	LCD_str(&arr[0]);                   //LCD string display function
	HAL_Delay(1000);
	LCD(0x80, 0);                       //cursor position to start of first line
	LCD_str("Enter password");          //display "Enter password" on first line
	LCD(0xC0, 0);                       //cursor position to start of second line
  while (1)
  {
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
		HAL_Delay(300);
		var1 = key_press();
		Disp_pass_key(var1);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
		HAL_Delay(1000);
	}
}