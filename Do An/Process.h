#ifndef _PROCESS_H
#define _PROCESS_H
#include "Airplane.h"


//-------------------DATABASE--------------------------------
void LoadAirplane(PTR_LIST_AIRPLANE &pListAirplane)
{
	int SizeAirplane = sizeof(AIRPLANE);
	fstream fileData("data.txt", ios::in | ios::binary);
	while (!fileData.eof())
	{
		++pListAirplane->n;
		pListAirplane->listAirplane[pListAirplane->n] = new AIRPLANE();
		fileData.read(reinterpret_cast<char*>(pListAirplane->listAirplane[pListAirplane->n]), SizeAirplane);
	}
	delete pListAirplane->listAirplane[pListAirplane->n];
	--pListAirplane->n;
	fileData.close();
}

void SaveAirplane(PTR_LIST_AIRPLANE &pListAirplane)
{
	fstream fileData("data.txt", ios::out | ios::binary);
	for (int i = 0; i <= pListAirplane->n; i++)
	{
		fileData.write(reinterpret_cast<const char*>(pListAirplane->listAirplane[i]), sizeof(AIRPLANE));
	}
	fileData.close();
	delete pListAirplane;
}

void WritePassengerToFile(NODE_PASSENGER* p, fstream &file, int size)
{
	if (p != NULL)
	{
		file.write(reinterpret_cast<const char*>(&p->data), size);
		WritePassengerToFile(p->pLeft, file, size);
		WritePassengerToFile(p->pRight, file, size);
	}
}
void SavePassenger(TREE_PASSENGER t)
{
	fstream myfile("passenger.txt", ios::out | ios::binary);
	myfile << nPassenger;
	int sizeData = sizeof(PASSENGER);
	TREE_PASSENGER p = t;
	WritePassengerToFile(p, myfile, sizeData);
	myfile.close();
}
void LoadPassenger(TREE_PASSENGER &t)
{
	fstream myfile("passenger.txt", ios::in | ios::binary);
	int nPassenger;
	myfile >> nPassenger;
	int sizeData = sizeof(PASSENGER);
	PASSENGER pa;
	for (int i = 0; i <= nPassenger; i++)
	{
		myfile.read(reinterpret_cast<char*>(&pa), sizeData);
		InsertPassengerToTree(t, pa);
	}
}

void LoadListFlight(LIST_FLIGHT &l)
{
	int SizeFlight = sizeof(FLIGHT) - sizeof(TICKET*);
	ifstream fileData("data_flight.txt", ios::in | ios::binary);
	while (!fileData.eof())
	{
		FLIGHT p;
		fileData.read(reinterpret_cast<char*>(&p), SizeFlight);
		p.listTicket = new TICKET[MAX_TICKET];
		for (int i = 0; i < p.nTicket; i++)
			fileData.read(reinterpret_cast<char*>(&p.listTicket[i]), sizeof(TICKET));
		AddTailListFlight(l, p);
	}
	if (l.n == 1) {
		InitListFlight(l);
		l.n = 0;
	} else{
		IsDeletedTail(l);
	}
	
	fileData.close();
}

void SaveFlight(LIST_FLIGHT fl)
{
	fstream fileData("data_flight.txt", ios::out | ios::binary);
	int SizeFlight = sizeof(FLIGHT) - sizeof(TICKET*);
	for (NODE_FLIGHT* k = fl.pHead; k != NULL; k = k->pNext) {
		fileData.write(reinterpret_cast<const char*>(&k->data), SizeFlight);
		for (int i = 0; i < k->data.nTicket; i++)
			fileData.write(reinterpret_cast<const char*>(&k->data.listTicket[i]), sizeof(TICKET));
	}
	fileData.close();
}
//--------------------------END DATABASE

//--------------------------STATISTIC
void OutputPassengerOnFlight(FLIGHT fl, TREE_PASSENGER t)
{
	int ordinal = -1;
	int stt = 0;
	Display(keyDisplayPassengerOnFlight, sizeof(keyDisplayPassengerOnFlight) / sizeof(string));
	DeleteGuide(sizeof(keyDisplayPassengerOnFlight) / sizeof(string) + 1);
	for (int i = 0; i < fl.nTicket; i++)
	{
		if (fl.listTicket[i].stt == TICKET_SOLD)
		{
			NODE_PASSENGER* pa = FindPassenger(t, fl.listTicket[i].idOwner);
			OutputPassengerOnFlightDetail(pa->data, fl.listTicket[i].numberTicket,++stt, ++ordinal);
		}
	}
}

void OutputFlightStartInDateAndDestiny(LIST_FLIGHT lf)
{
	if (lf.pHead == NULL && lf.pTail == NULL) return;
	int count = -1;
	for (NODE_FLIGHT* q = lf.pHead; q != NULL; q = q->pNext)
	{
		count++;
		OutputFlightByDateAndDestiny(q->data, count);
	}
}
bool StatisticPassengerOnFlightIsSucceed(PTR_LIST_AIRPLANE l, LIST_FLIGHT lf, TREE_PASSENGER t)
{
	int indexAirplane = DisplayAndChooseIndexAirplane(l, "CHON MAY BAY MUON THONG KE");
	if (indexAirplane == -1)
	{
		system("cls");
		return false;
	}
	CHOOSEAP:
	system("cls");
	Gotoxy(X_TITLE, Y_TITLE); cout << "CHON CHUYEN BAY MUON HIEN THI";
	LIST_FLIGHT tempLF = findByIdMB(lf, l->listAirplane[indexAirplane]->id);
	int tempNChair = l->listAirplane[indexAirplane]->soDay*l->listAirplane[indexAirplane]->soDong;
	totalPageFlight = (tempLF.n / QUANTITY_PER_PAGE) + 1;
	NODE_FLIGHT* fl = NULL;
	if (tempLF.n == 0) {
		Gotoxy(X_NOTIFY - 40, Y_NOTIFY); cout << "MAY BAY HIEN KHONG CO CHUYEN, NHAN PHIM BAT KY DE QUAY LAI MENU";
		_getch();
		goto CHOOSEAP;
	}	
	Display(keyDisplayFlight, sizeof(keyDisplayFlight) / sizeof(string));
	fl = ChooseFlight(tempLF);
	system("cls");
	Gotoxy(X_TITLE, Y_TITLE);
	cout << "DANH SACH HANH KHACH THUOC CHUYEN BAY " << fl->data.id;
	Gotoxy(X_TITLE, Y_TITLE + 1);
	cout << "NOI DEN " << fl->data.destiny;
	Gotoxy(X_TITLE, Y_TITLE + 2);
	cout << "NGAY KHOI HANH " << fl->data.dateLeave.d << "/" << fl->data.dateLeave.m << "/" << fl->data.dateLeave.y << " " << fl->data.dateLeave.h << ":" << fl->data.dateLeave.mi;
	
	if (fl->data.nTicketSold == 0)
	{
		Gotoxy(X_NOTIFY, Y_NOTIFY);
		cout << "Khong co du lieu";
	}
	else OutputPassengerOnFlight(fl->data, t);
	deleteListFlight(tempLF);
	_getch();
	return true;
}

bool StatisticFlightOnDay(LIST_FLIGHT lf)
{
	system("cls");
	DATETIME dt;
	string destiny;
	Gotoxy(X_ADD + 12 + 8, 3 + Y_ADD);
	cout << "/";
	Gotoxy(X_ADD + 12 + 11, 3 + Y_ADD);
	cout << "/";
	bool isMove = false;
	bool isSave = false;
	int newOrdinal = 0;
	Gotoxy(X_TITLE+10, Y_TITLE);
	cout << "NHAP THONG TIN ";
	Gotoxy(X_TITLE + 40, Y_TITLE+5);
	cout << "Nhap san bay: ";
	Gotoxy(X_TITLE + 40, Y_TITLE+8);
	cout << "Nhap ngay: ";
	Gotoxy(X_TITLE+10, Y_TITLE + 1);
	cout << "Nhan F10 de hien thi ket qua";
	while (true)
	{
		switch (newOrdinal)
		{
		case 0:
			CheckMoveAndValidateName(destiny, isMove, newOrdinal, isSave, 15);
			break;
		case 1:
			break;
		case 2:
			CheckMoveAndValidateDateTime(dt.d, isMove, newOrdinal, isSave, 31, 12);
			break;
		case 3:
			CheckMoveAndValidateDateTime(dt.m, isMove, newOrdinal, isSave, 12, 12);
			break;
		case 4:
			CheckMoveAndValidateDateTime(dt.y, isMove, newOrdinal, isSave, 10000, 12);
			break;
		} // end switch newordinal
		//check key move
		if (isMove)
		{
			if (newOrdinal == 0) 
				isMove = false;
			else
			newOrdinal--;
		}
		else
		{
			if (newOrdinal == 5)
				isMove = true;
			else
			newOrdinal++;
		}
		if (isSave)
		{
			if (DateTimeIsRightFormat(dt))
				break;
			Gotoxy(X_NOTIFY, Y_NOTIFY);
			cout << "Ngay gio k hop le. Nhan 1 phim bat";
			_getch();
			Gotoxy(X_NOTIFY, Y_NOTIFY);
			cout << setfill(' ') << setw(30) << " ";
			isSave = false;
		}
			
	} // end while
	ShowCur(false);
	system("cls");
	
	Gotoxy(X_TITLE, Y_TITLE + 2);
	cout << "CAC CHUYEN BAY KHOI HANH " << dt.d << "/" << dt.m << "/" << dt.y;
	
	int index = -1;
	bool isHaveData = false;;
	LIST_FLIGHT tempFL = findByIdDate(lf, dt, destiny);
	if (tempFL.n>0)
	{
		Display(keyDisplayFlightOnDateAndPlace, sizeof(keyDisplayFlightOnDateAndPlace) / sizeof(string));
		DeleteGuide(sizeof(keyDisplayFlightOnDateAndPlace) / sizeof(string));
		OutputFlightStartInDateAndDestiny(tempFL);
	}
	else
	{
		Gotoxy(X_NOTIFY, Y_NOTIFY);
		cout << "Khong co chuyen bay";
	}
	_getch();
	return true;
}
bool StatisticTicketFlight(PTR_LIST_AIRPLANE l, LIST_FLIGHT lf)
{
	int indexAirplane = DisplayAndChooseIndexAirplane(l, "CHON MAY BAY MUON THONG KE");
	if (indexAirplane == -1)
	{
		system("cls");
		return false;
	}
CHOOSEAP:
	system("cls");
	Gotoxy(X_TITLE, Y_TITLE); cout << "CHON CHUYEN BAY MUON HIEN THI";
	LIST_FLIGHT tempLF = findByIdMB(lf, l->listAirplane[indexAirplane]->id);
	int tempNChair = l->listAirplane[indexAirplane]->soDay*l->listAirplane[indexAirplane]->soDong;
	totalPageFlight = (tempLF.n / QUANTITY_PER_PAGE) + 1;
	NODE_FLIGHT* fl = NULL;
	NODE_FLIGHT* flSave;
	if (tempLF.n == 0) {
		Gotoxy(X_NOTIFY - 40, Y_NOTIFY); cout << "MAY BAY HIEN KHONG CO CHUYEN, NHAN PHIM BAT KY DE QUAY LAI MENU";
		_getch();
		goto CHOOSEAP;
	}
	Display(keyDisplayFlight, sizeof(keyDisplayFlight) / sizeof(string));
	fl = ChooseFlight(tempLF);
	system("cls");

	
	Gotoxy(X_TITLE, Y_TITLE); cout << "DANH SACH VE TRONG CUA CHUYEN BAY " << fl->data.id;
	cout << "  MAY BAY " << l->listAirplane[indexAirplane]->id;
	
	DisplayMenuTicket(fl->data, l->listAirplane[indexAirplane]->soDay, l->listAirplane[indexAirplane]->soDong);
	_getch();
	return true;
}
void QuickSortNumFlightOfAirPlain(int left, int right, N_FLIGHT_AIRPLANE a[]) {
	int key = a[(left + right) / 2].n;
	int i = left, j = right;
	do {
		while (a[i].n < key) i++;
		while (a[j].n > key) j--;
		if (i <= j) {
			if (i < j) swap(a[i], a[j]);
			i++;	j--;
		}
	} while (i <= j);
	if (left < j) QuickSortNumFlightOfAirPlain(left, j, a);
	if (right > i) QuickSortNumFlightOfAirPlain(i, right, a);
}
bool StatisticNumberFlightOfAirplane(PTR_LIST_AIRPLANE l, LIST_FLIGHT lf)
{
	
	N_FLIGHT_AIRPLANE sortList[MAX_AIRPLANE];
	system("cls");
	Gotoxy(X_TITLE, Y_TITLE); cout << "THONG KE CAC CHUYEN BAY CUA MAY BAY";
	Display(keyDisplaynumberFlightOfAirPlain, sizeof(keyDisplaynumberFlightOfAirPlain) / sizeof(string));
	DeleteGuide(3);
	for (int i = 0; i <= l->n; i++)
	{
		strcpy(sortList[i].idMB, l->listAirplane[i]->id);
		strcpy(sortList[i].type, l->listAirplane[i]->loaiMB);
		sortList[i].n = findByIdMB(lf, l->listAirplane[i]->id).n;
	}
	QuickSortNumFlightOfAirPlain(0, l->n, sortList);
	for (int i = l->n; i >= 0; i--)
	{
		int j = l->n - i;
		Gotoxy(xKeyDisplay[0] + 1, Y_DISPLAY + 3 + j); cout << sortList[i].idMB;
		Gotoxy(xKeyDisplay[1] + 1, Y_DISPLAY + 3 + j); cout << sortList[i].type;
		Gotoxy(xKeyDisplay[2] + 1, Y_DISPLAY + 3 + j); cout << sortList[i].n;
	}
	_getch();
	return true;
}


//--------------END STATISTIC



bool BookTicket(PTR_LIST_AIRPLANE l, TREE_PASSENGER &t, LIST_FLIGHT &lf)
{
	system("cls");
	Gotoxy(X_TITLE, Y_TITLE); cout << "CHON HANH KHACH MUON DAT VE";

	Display(keyDisplayPassenger, sizeof(keyDisplayPassenger) / sizeof(string));
	DeleteNote(sizeof(keyDisplayPassenger) / sizeof(string));
	NODE_PASSENGER* chosenPassenger = ChoosePassenger(t);
	if (chosenPassenger == NULL)
	{
		system("cls");
		return false;
	}
	CHOOSEAP:
	system("cls");
	int indexAirplane = DisplayAndChooseIndexAirplane(l, "CHON MAY BAY MUON DAT VE");
	if (indexAirplane == -1)
	{
		system("cls");
		return false;
	}
	system("cls");
	Gotoxy(X_TITLE, Y_TITLE); cout << " CHON CHUYEN BAY";
	LIST_FLIGHT tempLF = findByIdMB(lf, l->listAirplane[indexAirplane]->id);
	totalPageFlight = (tempLF.n / QUANTITY_PER_PAGE) + 1;
	NODE_FLIGHT* fl; //= NULL;
	NODE_FLIGHT* flSave;
	do
	{
		if (tempLF.n == 0) {
			Gotoxy(X_NOTIFY-40, Y_NOTIFY); cout << "MAY BAY HIEN KHONG CO CHUYEN, NHAN PHIM BAT KY DE QUAY LAI MENU";
			_getch();
			goto CHOOSEAP;
		}
		Display(keyDisplayFlight, sizeof(keyDisplayFlight) / sizeof(string));
		fl = ChooseFlight(tempLF);
		flSave = FindFlight(lf, fl->data.id);
		for (int i = 0; i < fl->data.nTicket; i++) {
			if (fl->data.listTicket[i].idOwner == chosenPassenger->data.id) {
				Gotoxy(X_NOTIFY, Y_NOTIFY); cout << "BAN DA DAT VE TREN CHUYEN BAY NAY!";
				_getch();
				goto CHOOSEAP;
			}
		}
		if (fl->data.stt != STT_REMAIN_TICKET)
		{
			Gotoxy(X_NOTIFY, Y_NOTIFY); cout << "CHON CHUYEN CON VE VA CHUA HOAN TAT";
			_getch();
		}
	} while (fl->data.stt != STT_REMAIN_TICKET && fl != NULL);
	if (fl == NULL)
	{
		system("cls");
		return false;
	}
	int k;
	system("cls");
	do
	{
		k = ChooseTicket(fl->data, l->listAirplane[indexAirplane]->soDay, l->listAirplane[indexAirplane]->soDong);
		if (fl->data.listTicket[k - 1].stt == TICKET_SOLD && k != -1)
		{
			Gotoxy(X_NOTIFY, Y_NOTIFY); cout << "VE DA CO NGUOI DAT";
			_getch();
			DeleteNotify();
		}
		
	} while (fl->data.listTicket[k - 1].stt == TICKET_SOLD && k != -1);
	if (k == -1)
	{
		system("cls");
		return false;
	}
	fl->data.listTicket[k - 1].stt = TICKET_SOLD;
	fl->data.nTicketSold++;
	fl->data.listTicket[k - 1].idOwner = chosenPassenger->data.id;
	chosenPassenger->data.isBooked++;
	if (fl->data.nTicket == fl->data.nTicketSold) fl->data.stt = STT_FULL_TICKET;
	flSave->data = fl->data;
	Gotoxy(X_NOTIFY, Y_NOTIFY); cout << "DAT VE THANH CONG, NHAN PHIM BAT KY DE QUAY LAI MENU";
	_getch();
	deleteListFlight(tempLF);
	return true;
}
void OutputTicket(TICKET tk, int locate)
{
	DeleteOldData(sizeof(keyDisplayTicket) / sizeof(string), locate);
	Gotoxy(xKeyDisplay[0] + 1, Y_DISPLAY + 3 + locate); cout << tk.idOwner;
	Gotoxy(xKeyDisplay[1] + 1, Y_DISPLAY + 3 + locate); cout << tk.numberTicket;
	Gotoxy(xKeyDisplay[2] + 1, Y_DISPLAY + 3 + locate); cout << tk.stt;
}
void SetDefaultChosenTicketCancel(TICKET tk, int ordinal)
{
	SetBGColor(GREEN);
	OutputTicket(tk, (ordinal % QUANTITY_PER_PAGE) * 2);
	SetBGColor(PURPLE);
}
void EffectiveMenuTicketCancel(int ordinal, TICKET* l)
{
	int current = ordinal;
	SetDefaultChosenTicketCancel(l[currposTicket], current);
	OutputTicket(l[currposPreTicket], (currposPreTicket % QUANTITY_PER_PAGE) * 2);
	currposPreTicket = current;
}
void OutputListTicketCancelPerPage(TICKET* l, int indexBegin, int totalIndex)
{
	int count = 0;

	for (int i= indexBegin; i< totalIndex;i++)
	{
		if (count < QUANTITY_PER_PAGE)
		{
			OutputTicket(l[i], (count) * 2);
			++count;
		}
		else {
			break;
		}
	}
	Gotoxy(X_PAGE, Y_PAGE);
	cout << "Trang " << pageNowFlight << "/" << totalPageFlight;
	return;


}
void ChangePageTicketCancel(TICKET *l)
{
	system("cls");
	Display(keyDisplayTicket, sizeof(keyDisplayTicket) / sizeof(string));
	DeleteNote(sizeof(keyDisplayTicket) / sizeof(string));
	currposTicket = (pageNowTicket - 1) * QUANTITY_PER_PAGE;
	currposPreTicket = (pageNowTicket - 1) * QUANTITY_PER_PAGE;
	Gotoxy(X_PAGE, Y_PAGE);
	cout << "Trang " << pageNowTicket << "/" << totalPageTicket;
}
TICKET* findTicketSold(NODE_FLIGHT* l) {
	TICKET* tempView = new TICKET[l->data.nTicketSold];
	int index = -1;
	for (int i = 0; i < l->data.nTicket; i++) {
		if (l->data.listTicket[i].stt == TICKET_SOLD) {
			tempView[++index] = l->data.listTicket[i];
		}
		if (index == l->data.nTicketSold - 1) {
			break;
		}
	}
	return tempView;
}

bool ChooseCancelTicket(NODE_FLIGHT* &l, TREE_PASSENGER &t)
{
	TICKET* tempView = findTicketSold(l);
	int keyboard_read = 0;
	ShowCur(false);
	pageNowFlight = 1;
	currposTicket = (pageNowTicket - 1) * QUANTITY_PER_PAGE;
	currposPreTicket = (pageNowTicket - 1) * QUANTITY_PER_PAGE;
	totalPageTicket = ((l->data.nTicketSold - 1) / QUANTITY_PER_PAGE) + 1;
	OutputListTicketCancelPerPage(tempView, (pageNowFlight - 1) * QUANTITY_PER_PAGE, l->data.nTicketSold);
	SetDefaultChosenTicketCancel(tempView[currposTicket], currposTicket);
	while (true)
	{
		// xu ly bat phim len xuong
		while (_kbhit())
		{
			keyboard_read = _getch();
			if (keyboard_read == 0)
				keyboard_read = _getch();
			switch (keyboard_read)
			{
			case KEY_UP:
				if (currposTicket % QUANTITY_PER_PAGE > 0)
				{
					currposTicket = currposTicket - 1;
					EffectiveMenuTicketCancel(currposTicket, tempView);
				}
				break;
			case KEY_DOWN:
				if (currposTicket % QUANTITY_PER_PAGE < QUANTITY_PER_PAGE - 1 && currposTicket < l->data.nTicketSold-1)
				{
					currposTicket = currposTicket + 1;
					EffectiveMenuTicketCancel(currposTicket, tempView);
				}
				break;
			case PAGE_DOWN:
				if (pageNowTicket < totalPageTicket)
				{

					pageNowTicket++;
					ChangePageTicketCancel(tempView);
					OutputListTicketCancelPerPage(tempView, (pageNowAirplane - 1) * QUANTITY_PER_PAGE, l->data.nTicketSold);
					SetDefaultChosenTicketCancel(tempView[currposTicket], currposTicket);

				}
				break;
			case PAGE_UP:
				if (pageNowTicket > 1)
				{

					pageNowTicket--;
					ChangePageTicketCancel(tempView);
					OutputListTicketCancelPerPage(tempView, (pageNowAirplane - 1) * QUANTITY_PER_PAGE, l->data.nTicketSold);
					SetDefaultChosenTicketCancel(tempView[currposTicket], currposTicket);

				}
				break;
			case ESC:
				ShowCur(false);
				return false;
			case ENTER:
				ShowCur(false);
				for (int i = 0; i < l->data.nTicket; i++) {
					if (l->data.listTicket[i].idOwner == tempView[currposTicket].idOwner) {
						l->data.listTicket[i].stt = TICKET_EMPTY;
						l->data.nTicketSold--;
						l->data.listTicket[i].idOwner = 0;
						// giam check booking cua passenger
						FindPassenger(t, tempView[currposTicket].idOwner)->data.isBooked--;
						// update lai status flight
						if (l->data.stt == 2) {
							l->data.stt == 1;
						}
					}
					DeleteOldData(sizeof(keyDisplayTicket) / sizeof(string), (currposTicket%QUANTITY_PER_PAGE)*2);
				}
				return true;
			}
		}

	}
}

bool CancelTicket(PTR_LIST_AIRPLANE l, TREE_PASSENGER &t, LIST_FLIGHT &lf)
{
	int indexAirplane = DisplayAndChooseIndexAirplane(l, "CHON MAY BAY");
	if (indexAirplane == -1)
	{
		system("cls");
		return false;
	}
	system("cls");
	Gotoxy(X_TITLE, Y_TITLE); cout << "CHON CHUYEN BAY";
	LIST_FLIGHT tempLF = findByIdMB(lf, l->listAirplane[indexAirplane]->id);
	int tempNChair = l->listAirplane[indexAirplane]->soDay*l->listAirplane[indexAirplane]->soDong;
	totalPageFlight = (tempLF.n / QUANTITY_PER_PAGE) + 1;
	NODE_FLIGHT* fl = NULL;
	NODE_FLIGHT* flSave = NULL;

	if (tempLF.n == 0) {
		Gotoxy(X_NOTIFY - 40, Y_NOTIFY); cout << "MAY BAY HIEN KHONG CO CHUYEN, NHAN PHIM BAT KY DE QUAY LAI MENU";
		_getch();
		return false;
	}
	Display(keyDisplayFlight, sizeof(keyDisplayFlight) / sizeof(string));
	DeleteNote(5);
	fl = ChooseFlight(tempLF);
	flSave = FindFlight(lf, fl->data.id);
	if (fl->data.stt != STT_REMAIN_TICKET)
	{
		Gotoxy(X_NOTIFY, Y_NOTIFY); cout << "KHONG THE HUY VE CUA CHUYEN HOAN TAT HOAC BI HUY";
		_getch();
		system("cls");
		return false;
	}
	system("cls");
	Gotoxy(X_TITLE, Y_TITLE);
	cout << "DANH SACH HANH KHACH VA VE THUOC CHUYEN BAY";

	if (fl->data.nTicketSold == 0)
	{
		Gotoxy(X_NOTIFY, Y_NOTIFY);
		cout << "Khong co du lieu";
		_getch();
		system("cls");
		return false;
	}
	else {
		Display(keyDisplayTicket, sizeof(keyDisplayTicket) / sizeof(string));
		DeleteNote(3);
		Gotoxy(X_NOTIFY, Y_NOTIFY);
		cout << "Chon ve + ENTER de huy ve, ESC -> menu";
		bool check = ChooseCancelTicket(fl,t);
		if (check) {
			flSave->data = fl->data;
			delete fl;
			DeleteNotify();
			Gotoxy(X_NOTIFY, Y_NOTIFY);
			cout << "Huy Ve Thanh Cong";
		}
	}
	_getch();
	return true;
}

void Introduce()
{
	ShowCur(false);
	string a;
	char b;
	ifstream file("introduce.txt", ios::in);
	while (!file.eof())
	{
		getline(file, a);
		cout << a << endl;
	}
	Gotoxy(100, 35);
	cout << "Vo Huy Hoang";
	Gotoxy(100, 36);
	cout << "N13DCCN060";
	Gotoxy(100, 37);
	cout << "D13CQIS01-N";
}

void MageAll(PTR_LIST_AIRPLANE &l, TREE_PASSENGER &t, LIST_FLIGHT &lf)
{
	while (true)
	{
		Introduce();
		MainMenu(keyMainMenu, sizeof(keyMainMenu) / sizeof(string));
		int type = ChooseMainMenu(keyMainMenu, sizeof(keyMainMenu) / sizeof(string));
		AutoChangeSttFlight(lf);
		totalPageAirplane = l->n / QUANTITY_PER_PAGE + 1;
		totalPagePassenger = nPassenger / QUANTITY_PER_PAGE + 1;
		switch (type)
		{
		case -1:
			return;
		case 0:
		{
			MenuManageAirplane(l,lf);
			break;
		}
		case 1:
		{
			int indexairplanee = DisplayAndChooseIndexAirplane(l, "CHON MAY BAY");
			if (indexairplanee == -1)
			{
				system("cls");
				continue;
			}
			system("cls");
			Gotoxy(X_TITLE, Y_TITLE); cout << " quan ly chuyen bay cuay may bay co ma so " << l->listAirplane[indexairplanee]->id;
			LIST_FLIGHT tempLF = findByIdMB(lf, l->listAirplane[indexairplanee]->id);
			totalPageFlight = (tempLF.n / QUANTITY_PER_PAGE) + 1;

			MenuManageFlight(l->listAirplane[indexairplanee]->id, l->listAirplane[indexairplanee]->soDay, l->listAirplane[indexairplanee]->soDong, lf, tempLF, t);
			break;
		}
		case 2:
		{
			system("cls");
			Gotoxy(X_TITLE, Y_TITLE); cout << " QUAN LY HANH KHACH ";
			MenuManagePassenger(t);
			break;
		}
		case 3:
			if (BookTicket(l, t, lf) == false) continue;
			break;
		case 4:
			if (CancelTicket(l, t, lf) == false) continue;
			break;
		case 5:
		{
			system("cls");
			StatisticMenu(keyStatistic, sizeof(keyStatistic) / sizeof(string));
			int chosenStatistic = ChooseStatisticMenu(keyStatistic, sizeof(keyStatistic) / sizeof(string));
			switch (chosenStatistic)
			{
			case 0:
				if (StatisticPassengerOnFlightIsSucceed(l,lf,t) == false)	continue;
				break;
			case 1:
				if (StatisticFlightOnDay(lf) == false)	continue;
				break;
			case 2:
				if (StatisticNumberFlightOfAirplane(l,lf) == false)	continue;
				break;
			case 3:
				if (StatisticTicketFlight(l,lf) == false)	continue;
				break;
			default:
				break;
			}
		}//end case 4 of type			
		}
		system("cls");
	}//while true
}
#endif