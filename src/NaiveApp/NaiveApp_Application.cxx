#include "NaiveApp_Application.hxx"

#include <TPrsStd_DriverTable.hxx>
#include <XCAFPrs_Driver.hxx>

void NaiveApp_InitPrsDrivers() {
  Handle(TPrsStd_DriverTable) aDriverTable = TPrsStd_DriverTable::Get();
  aDriverTable->InitStandardDrivers();
  aDriverTable->AddDriver(XCAFPrs_Driver::GetID(), new XCAFPrs_Driver);
}
