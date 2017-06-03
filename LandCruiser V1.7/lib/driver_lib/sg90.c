
//»ÔÊ¤SG90¶æ»ú¿ØÖÆ
#include "sg90.h"
#include "stm32f10x.h"


/**
  * @brief  60 maxUp, 140 maxDown, 80 mid 
  * @param  sg90_up_down(SG90_UD_030);   
  * @retval no
  */
void sg90_up_down(u8 chg_val)
{
	SG90_TIM->CCR3 = chg_val;
}
/**
  * @brief  sg90_left_right(SG90_LR_090);	
  * @param  
  * @retval no
  */
void sg90_left_right(u8 chg_val)
{	
	SG90_TIM->CCR4 = chg_val;
}
/**
  * @brief  
  * @param  ¸´Î»Öµ 
  * @retval   sg90_reset(80, 100);   //¸´Î»×´Ì¬  
  */
void sg90_reset(u8 ud_val, u8 lr_val)
{
	SG90_TIM->CCR3 = ud_val;
	SG90_TIM->CCR4 = lr_val;
}
