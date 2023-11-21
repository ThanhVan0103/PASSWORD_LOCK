unsigned char key_press(void)
{
	unsigned char clm_var;
	unsigned char key_arr[] = {7, 8, 9, 15, 4, 5, 6, 14, 1, 2, 3, 13, 10, 0, 11, 12};
		
	if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == 0)
	{
		clm_var = check_clm(GPIO_PIN_0);
		return key_arr[clm_var-1];
	}
	else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == 0)
	{
		clm_var = check_clm(GPIO_PIN_1);
		return key_arr[clm_var+3];
	}
	else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2) == 0)
	{
		clm_var = check_clm(GPIO_PIN_2);
		return key_arr[clm_var+7];
	}
	else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3) == 0)
	{
		clm_var = check_clm(GPIO_PIN_3);
		return key_arr[clm_var+11];
	}
	else 
		return 16;	
}

unsigned char check_clm(unsigned char pin_var)
{
	unsigned char clm;
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_SET);		//Set all columns to logic 1 ie connected to Vcc
	
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);						//set first column to Gnd
	if(HAL_GPIO_ReadPin(GPIOB, pin_var) == 0)
		clm = 1;
	
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);						//set second column to Gnd
	if(HAL_GPIO_ReadPin(GPIOB, pin_var) == 0)
		clm = 2;
	
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);						//set third column to Gnd
	if(HAL_GPIO_ReadPin(GPIOB, pin_var) == 0)
		clm = 3;
	
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);						//set fourth column to Gnd
	if(HAL_GPIO_ReadPin(GPIOB, pin_var) == 0)
		clm = 4;
	
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);		//Set all columns to logic 0 ie connected to GND
	return clm; 
}

void LCD_init(void)
{
	LCD(0x38, 0);			//2 lines, 5*7 matrix
	//LCD(0x0C, 0);			//Display on, cursor off
	LCD(0x0E, 0);			//Display on, cursor blinking
	LCD(0x06, 0);			//Increment cursor (shift cursor to right)
	LCD(0x01, 0);			//Clear display screen
	LCD(0x80, 0);			//Force cursor to beginning ( 1st line)
}
	
void LCD(uint8_t val_1, uint8_t cmd)
{
	uint8_t data1;
	
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, cmd);			//set RS = cmd; (cmd=0) => Command; (cmd=1) => Data
	
	data1 = val_1 & 0x01;														//mask bit and assign it to GPIO 0 
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, data1);
	
	data1 = (val_1>>1) & 0x01;											//mask bit and assign it to GPIO 1
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, data1); 
	
	data1 = (val_1>>2) & 0x01;											//mask bit and assign it to GPIO 2
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, data1);
	
	data1 = (val_1>>3) & 0x01;											//mask bit and assign it to GPIO 3
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, data1);
	
	data1 = (val_1>>4) & 0x01;											//mask bit and assign it to GPIO 4
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, data1);
	
	data1 = (val_1>>5) & 0x01;											//mask bit and assign it to GPIO 5
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, data1);
	
	data1 = (val_1>>6) & 0x01;											//mask bit and assign it to GPIO 6
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, data1);
	
	data1 = (val_1>>7) & 0x01;											//mask bit and assign it to GPIO 7
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, data1);
	
	//---------------------------------------------//
	//		Enable Pulse		//
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
	HAL_Delay(5);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
}

void LCD_str(unsigned char *str1)
{
	while(*str1 != 0)               //0 ==> null character
	{
		LCD(*str1, 1);              //*str1 : value at the address pointed
		str1++;
	}
}
	
void Disp_pass_key(unsigned char key_var)
{
	static unsigned char pass_var1 = 0;
	static unsigned char key_arr[5];
	unsigned char enter_key[5] = {"12345"};         //correct passkey
	unsigned char i;
	
	if(key_var != 16)
	{
		key_arr[pass_var1] = key_var + 48;
		LCD(key_var + 48, 1);
		pass_var1++;
		if(pass_var1 == 5)
		{
			LCD(0xC0, 0);
			for(i=0;i<5;i++)
			{
				if(key_arr[i] != enter_key[i])
				{	
					LCD_str("Re-enter pw");
					pass_var1 = 0;
					break;
				}
			}
			if(i == 5)
			{
				LCD_str("Welcome!!!!");
			}
			HAL_Delay(3000);
			pass_var1 = 0;
			LCD(0x01, 0);
			LCD(0x80, 0);
			LCD_str("Enter password:");
			LCD(0xC0, 0);
		}
		
	}
}