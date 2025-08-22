#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ATExample, "Project_Urban.Example",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::CriticalPriority | EAutomationTestFlags::ProductFilter)

bool ATExample::RunTest(const FString& Parameters)
{
	return true;
}
