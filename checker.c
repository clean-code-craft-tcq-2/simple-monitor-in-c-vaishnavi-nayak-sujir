#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define BMS_TEMP_WARNING 1// ENABLE TEMPERATURE EARLY WARNING
#define BMS_CHARGESTATE_WARNING 1 //ENABLE CHARGE STATE EARLY WARNING
#define BMS_CHARGERTATE_WARNING 1 //ENABLE CHARGE RATE EARLY WARNING


/* DEFINE THRESHOLD LIMITS FOR WARNINGS AND ERRORS*/
#define LOW_SOC_BREACH 20 
#define LOW_SOC_WARNING 24 
#define HIGH_SOC_WARNING 75 
#define HIGH_SOC_BREACH 80 
#define LOW_TEMP_BREACH 0 
#define LOW_TEMP_WARNING 5 
#define HIGH_TEMP_WARNING 40 
#define HIGH_TEMP_BREACH 45
#define CHARGE_RATE_WARNING 0.7F 
#define CHARGE_RATE_BREACH 0.8F

int temp_error_level;// 0: exceeds upper and lower threshold, 1: no error , 2: warning 
void print_warning_console(const char * alert_message)
{
	 printf("%s",alert_message);
	
}
float kelvin_to_celcius(float kelvin_temp)
{
	return kelvin_temp -273.15;
	
}
float farenhiet_to_celcius(float farenhiet_temp)
{
	return ((farenhiet_temp)- 32) * 5 / 9;;
	
}
float convert_temp_unitbased(char temp_unit , float temperature)
{
	float temp_celcius;
		switch(temp_unit)
	{/* convert any unit to celcius*/

		case 'k':
			temp_celcius = kelvin_to_celcius(temperature);
			break;
		case 'f':
			temp_celcius = farenhiet_to_celcius(temperature);
			break;
		default:
			temp_celcius = temperature;
		break;
			
	}
	return temp_celcius;
	
}

float convert_temp_celcius(char* input_temperature)
{
	char temp_unit = input_temperature[strlen(input_temperature)-1];// extract last char of string temperature which has units like k, f, c
	char temperature[strlen(input_temperature)-1];
	float temp_celcius;
	int i;
	for(i=0;i<strlen(input_temperature)-1;i++)
	{
		temperature[i]=input_temperature[i];// extract all chars of input temperature except last character that has units
	}
	temperature[i]='\0';	
	
	temp_celcius =  convert_temp_unitbased(temp_unit,atof(temperature));

	return temp_celcius;
	
}
int bms_temp_warning(float temperature)
{

	if(temperature < LOW_TEMP_WARNING ){
	print_warning_console("Early warning for lower temperature \n");	
	return 1;
	}
	else if(temperature > HIGH_TEMP_WARNING ){
	print_warning_console("Early warning for higher temperature \n");	
	return 1;
	}
	return 0;
}

int bms_temp_error(float temperature)
{
	temp_error_level =1;
	if(temperature < LOW_TEMP_BREACH) {
    	print_warning_console("temperature is below lower threshold\n");
    	temp_error_level= 0;
	}
	else if (temperature > HIGH_TEMP_BREACH){
		print_warning_console("temperature exceeds upper threshold\n");	
		temp_error_level= 0;
	}
#if (BMS_TEMP_WARNING == 1)
	if(temp_error_level !=0){//Issue warning only if above error condition not occured
	
	temp_error_level =  bms_temp_warning(temperature)?2:temp_error_level;
	}
#endif
	return temp_error_level;
}

int bms_chargestate_warning(float soc)
{
	if(soc < LOW_SOC_WARNING){
	print_warning_console("Early warning for charge state \n");	
	return 1;
	}
	else if(soc > HIGH_SOC_WARNING){
	print_warning_console("Early warning for higher charge state \n");	
	return 1;
	}
	return 0;
}

int bms_chargerate_warning(float chargeRate)
{
	if(chargeRate > CHARGE_RATE_WARNING){
	print_warning_console("Early warning for higher charge rate \n");	
	return 1;
	}
	return 0;
}


int batt_temp_range_check(char* temperature)
{
	float temp_celcius=  convert_temp_celcius(temperature);
	return bms_temp_error(temp_celcius);;
  
}

int batt_ChargeState_range_check(float soc)
{
	int soc_error_level=1;// 0: exceeds upper and lower threshold, 1: no error , 2: warning 
	if (soc < LOW_SOC_BREACH ){
		
	print_warning_console("ChargeState is below than lower threshold\n");
	
	soc_error_level=0;
	}
	else if( soc > HIGH_SOC_BREACH){
		
	print_warning_console("ChargeState exceeds upper threshold\n");
	
	soc_error_level=0;
	}
#if (BMS_CHARGESTATE_WARNING == 1)
	if (soc_error_level !=0){//Issue warning only if above error condition not occured
	soc_error_level =  bms_chargestate_warning(soc)?2:soc_error_level;
	}
#endif
	 return soc_error_level;
}

int batt_ChargeRateOk(float chargeRate)
{
	int chargeRate_error_level=1;// 0: exceeds upper and lower threshold, 1: no error , 2: warning 
	
	if(chargeRate > CHARGE_RATE_BREACH){
		
	print_warning_console("ChargeRate exceeds upper threshold\n");
	
	chargeRate_error_level =0;
	}	
#if (BMS_CHARGERTATE_WARNING == 1)
	if (chargeRate_error_level !=0){//Issue warning only if above error condition not occured
	chargeRate_error_level= bms_chargerate_warning(chargeRate)?2:chargeRate_error_level;
	}
#endif
	return chargeRate_error_level;
}

void test_bms_pass( char* temp,float chargestate,float chargerate) {
	int result ;
	result = batt_temp_range_check(temp) == 1&& batt_ChargeState_range_check(chargestate) ==1 &&batt_ChargeRateOk(chargerate) ==1;
	assert(result);
	
}

void test_bms_warning( char* temp,float chargestate,float chargerate) {
	int result ;
	result = batt_temp_range_check(temp) == 2 || batt_ChargeState_range_check(chargestate) ==2 || batt_ChargeRateOk(chargerate) ==2;
	assert(result);
	
}
void test_bms_fail( char* temp,float chargestate,float chargerate)  {
	int result ;
	result =!( batt_temp_range_check(temp)&& batt_ChargeState_range_check(chargestate)&&batt_ChargeRateOk(chargerate));
	assert(result);
	
}

int main() {
	
/* ALL PASS CONDITION*/
  test_bms_pass("25c",70,0.7F);//all pass condition
  test_bms_pass("298.15k",70,0.7F);//Test kelvin conversion
  test_bms_pass("77f",70,0.7f);//Test farenhiet conversion: all pass
  
/* TEMPERATURE LESS THAN LOWER THRESHOLD  CONDITION*/
  test_bms_fail("-5c",70,0.7f);//Test celcius :temperature lower threshold
  test_bms_fail("268.15k",70,0.7f);//Test kelvin conversion: temperature lower threshold
  test_bms_fail("23f",70,0.7f);//Test farenhiet :temperature lower threshold
  
/* TEMPERATURE GREATER THAN UPPER THRESHOLD  CONDITION*/ 
  test_bms_fail("323.15k",70,0.7f);//Test kelvin: temperature upper threshold
  test_bms_fail("50c",70,0.7f);//Test celcius: temperature upper threshold 
  test_bms_fail("122f",70,0.7f);//Test farenhiet: temperature upper threshold 
  
 /* TEMPERATURE WARNING CONDITION*/ 
  test_bms_warning("45c",70,0.7f);//Test celcius :temperature boundary check
  test_bms_warning("318.15k",70,0.7f);//Test celcius :Test kelvin:temperature boundary check
  test_bms_warning("113f",70,0.7f);//Test celcius :Test kelvin:temperature boundary check
  test_bms_warning("25c",70,0.75F);//temperature lower threshold early warning
  test_bms_warning("4c",70,0.7f);//temperature lower threshold early warning
  test_bms_warning("277.15k",70,0.7f);//Test kelvin:temperature lower threshold early warning
  test_bms_warning("42c",70,0.7f);//temperature upper threshold early 
  
 /* CHARGE STATE TEST  CONDITION*/   
  test_bms_fail("25c",19,0.7f);//charge state lower threshold
  test_bms_fail("25c",81,0.7f);//charge state upper threshold
  test_bms_warning("25c",20,0.7f);//charge state boundary check
  test_bms_warning("25c",70,0.8F);//charge rate boundary check
  test_bms_fail("25c",70,1.2F);//charge rate upper threshold*/

  test_bms_warning("25c",23,0.7f);//SOC lower threshold early warning
  test_bms_warning("25c",78,0.7f);//SOC upper threshold early warning





  
  
}
