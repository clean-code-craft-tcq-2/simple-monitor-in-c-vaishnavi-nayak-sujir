#include <stdio.h>
#include <assert.h>
void print_warning(const char * alert_message)
{
	 printf("%s out of range!\n",alert_message);
	
}

int batt_tempOk(float temperature)
{
	if(temperature < 0 || temperature > 45) {
    print_warning("temperature");
    return 0;
	}
	else return 1;
  
}

int batt_ChargeStateOk(float soc)
{
	if (soc < 20 || soc > 80){
	print_warning("ChargeState");
	return 0;}
	else return 1;
}

int batt_ChargeRateOk(float chargeRate)
{
	if(chargeRate > 0.8F){
	print_warning("ChargeRate");
	return 0;}	
	else return 1;
}

void test_bms_pass(int temp,int chargestate,int chargerate) {
	int result ;
	result = batt_tempOk(temp)&& batt_ChargeStateOk(chargestate)&&batt_ChargeRateOk(chargerate);
	assert(result);
	
}
void test_bms_fail(int temp,int chargestate,int chargerate)  {
	int result ;
	result =!( batt_tempOk(temp)&& batt_ChargeStateOk(chargestate)&&batt_ChargeRateOk(chargerate));
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
