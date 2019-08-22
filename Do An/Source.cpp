#include "Process.h"

int main()
{
	system("color 7A");

	setFullScreen();
	PTR_LIST_AIRPLANE l = new LIST_AIRPLANE;
	LoadAirplane(l);
	LIST_FLIGHT lf;
	InitListFlight(lf);
	LoadListFlight(lf);
	TREE_PASSENGER t;
	InitTreePassenger(t);
	LoadPassenger(t);
	MageAll(l,t,lf);
	SaveAirplane(l);
	SaveFlight(lf);
	SavePassenger(t);
	deleteListFlight(lf);
	FreeTreePassenger(t);
	system("pause");
	return 0;
}