#include <stdio.h>
#include <assert.h>
void print_warning_console(const char * alert_message)
{
	 printf("%s",alert_message);
	
}

int batt_temp_range_check(float temperature)
{
	if(temperature < 0) {
    	print_warning_console("temperature is below than lower threshold\n");
    	return 0;
	}
	else if (temperature > 45){
	print_warning_console("temperature exceeds upper threshold\n");	
	return 0;
	}
	else return 1;
  
}

int batt_ChargeState_range_check(float soc)
{
	if (soc < 20 ){
	print_warning_console("ChargeState is below than lower threshold\n");
	return 0;}
	else if( soc > 80){
	print_warning_console("ChargeState exceeds upper threshold\n");
	return 0;}
	else return 1;
}

int batt_ChargeRateOk(float chargeRate)
{
	if(chargeRate > 0.8F){
	print_warning_console("ChargeRate exceeds upper threshold\n");
	return 0;}	
	else return 1;
}

void test_bms_pass(int temp,int chargestate,int chargerate) {
	int result ;
	result = batt_temp_range_check(temp)&& batt_ChargeState_range_check(chargestate)&&batt_ChargeRateOk(chargerate);
	assert(result);
	
}
void test_bms_fail(int temp,int chargestate,int chargerate)  {
	int result ;
	result =!( batt_temp_range_check(temp)&& batt_ChargeState_range_check(chargestate)&&batt_ChargeRateOk(chargerate));
	assert(result);
	
}

int main() {
  test_bms_pass(25,70,0.7f);//all pass condition
  test_bms_fail(-5,70,0.7f);//temperature lower threshold
  test_bms_fail(50,70,0.7f);//temperature upper threshold 
  test_bms_pass(45,70,0.7f);//temperature boundary check
  test_bms_fail(25,19,0.7f);//charge state lower threshold
  test_bms_fail(25,81,0.7f);//charge state upper threshold
  test_bms_pass(25,20,0.7f);//charge state boundary check
  test_bms_pass(25,70,0.8F);//charge rate boundary check
  test_bms_fail(25,70,1.2F);//charge rate upper threshold*/

  
  
}
