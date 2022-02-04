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
	if(chargeRate > 0.8){
	print_warning("ChargeRate");
	return 0;}	
	else return 1;
}


int main() {
  int result = batt_tempOk(25)&& batt_ChargeStateOk(70)&&batt_ChargeRateOk(0.7);
  assert(result);
 // result = batt_tempOk(50)&& batt_ChargeStateOk(85)&&batt_ChargeRateOk(0);
 // assert(result);
}
