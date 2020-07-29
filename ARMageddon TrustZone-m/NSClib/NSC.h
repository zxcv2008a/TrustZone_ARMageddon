
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SECURE_NSC_H
#define SECURE_NSC_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported types ------------------------------------------------------------*/
/**
  * @brief  non-secure callback ID enumeration definition
  */
typedef enum
{
  SECURE_FAULT_CB_ID     = 0x00U, /*!< System secure fault callback ID */
  GTZC_ERROR_CB_ID       = 0x01U  /*!< GTZC secure error callback ID */
} SECURE_CallbackIDTypeDef;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void SECURE_RegisterCallback(SECURE_CallbackIDTypeDef CallbackId, void *func);

#endif /* SECURE_NSC_H */

/*#define CMSE_NS_CALL __cmse_nonsecure_call
#define CMSE_NS_ENTRY __cmse_nonsecure_entry
#define symbol NSC_start = 0x00000800;
#define symbol NSC_end = 0x000008FF;
#define region NSC_region = from NSC_start to NSC_end;

place in NSC_region { 
	section Veneer$$CMSE
 };

CMSE_NS_ENTRY uint32_t foo(uint32_t i)
{
return( foo() );
}
*/
