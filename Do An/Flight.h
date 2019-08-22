#ifndef _FLIGHT_H
#define _FLIGHT_H

#include "Passenger.h"
#include "Ticket.h"
#include <fstream>

struct Flight
{
	int nTicket;
	char id[15];
	char destiny[100]; 
	char idMB[15];
	DateTime dateLeave;
	int stt = STT_REMAIN_TICKET;
	int  nTicketSold = 0;
	Ticket *listTicket = new Ticket[MAX_TICKET];
};
typedef struct Flight FLIGHT;

struct NodeFlight
{
	FLIGHT data;
	struct NodeFlight *pNext;
};
typedef struct NodeFlight NODE_FLIGHT;

struct ListFlight
{
	int n = 0;
	NODE_FLIGHT *pHead, *pTail;
};
typedef struct ListFlight LIST_FLIGHT;


NODE_FLIGHT* GetNodeFlight(FLIGHT DATA)
{
	NODE_FLIGHT *p = new NODE_FLIGHT; // initialization pointer;
	if (p == NULL)
		return NULL;
	p->data = DATA; // export data to node;
	p->pNext = NULL; //init link list
	return p;
}
void InitListFlight(LIST_FLIGHT &l)
{
	sttFlight = -1;
	l.pHead = l.pTail = NULL;
}

bool ListFlightIsEmpty(ListFlight l)
{
	return l.pHead == NULL;
}

void AddHeadListFlight(LIST_FLIGHT &l, FLIGHT data)
{
	NODE_FLIGHT* p = GetNodeFlight(data);
	p->pNext = l.pHead;
	l.pHead = p;
	++l.n;
}

void AddTailListFlight(LIST_FLIGHT &l, FLIGHT data)
{
	NODE_FLIGHT *p = GetNodeFlight(data);
	if (l.pHead == NULL) {
		l.pHead = l.pTail = p;
	}
	 else
	{
		l.pTail->pNext = p;
		l.pTail = p;
	}
	++l.n;
}

NODE_FLIGHT* FindFlight(LIST_FLIGHT l, char *id)
{
	if (l.pHead == NULL) return NULL;
	for (NODE_FLIGHT *p = l.pHead ; p != NULL; p = p->pNext)
		if (_strcmpi(p->data.id, id) == 0)
			return p;
	return NULL;
}
NODE_FLIGHT* FindFlightByOridnal(LIST_FLIGHT l, int ordinal)
{
	if (l.pHead == NULL) return NULL;
	if (l.n -1 < ordinal) return NULL;
	if (l.n - 1 == ordinal) return l.pTail;
	if (ordinal == 0) return l.pHead;

	NODE_FLIGHT *p = l.pHead;
	int count = 0;
	while (count != ordinal)
	{
		p = p->pNext;
		count++;
	}
	return p;
}

int FindIndexFlight(LIST_FLIGHT l, char *id)
{
	int index = -1;
	if (l.pHead == NULL) return -1;
	for (NODE_FLIGHT *p = l.pHead; p != NULL; p = p->pNext)
	{
		++index;
		if (_strcmpi(p->data.id, id) == 0)
			return index;
	}
	return 0;
}
void InputDatimeee(DATETIME &dt)
{
	int newOrdinal = 0;
	bool newIsMoveUp = false;
	bool newIsSave = false;
	Gotoxy(X_ADD + 12 + 2, 3 + Y_ADD);
	cout << ":";
	Gotoxy(X_ADD + 12 + 8, 3 + Y_ADD);
	cout << "/";
	Gotoxy(X_ADD + 12 + 11, 3 + Y_ADD);
	cout << "/";
	while (true)
	{
 		switch (newOrdinal)
		{
		case 0:
			CheckMoveAndValidateDateTime(dt.h, newIsMoveUp, newOrdinal, newIsSave, 23, 12);
			break;
		case 1:
			CheckMoveAndValidateDateTime(dt.mi, newIsMoveUp, newOrdinal, newIsSave, 59, 12);
			break;
		case 2:
			CheckMoveAndValidateDateTime(dt.d, newIsMoveUp, newOrdinal, newIsSave, 31, 12);
			break;
		case 3:
			CheckMoveAndValidateDateTime(dt.m, newIsMoveUp, newOrdinal, newIsSave, 12, 12);
			break;
		case 4:
			CheckMoveAndValidateDateTime(dt.y, newIsMoveUp, newOrdinal, newIsSave, 10000, 12);
			break;
	
		} // end switch newordinal
		
		//check move
		if (newIsMoveUp)
		{
			if (newOrdinal == 0)
				newIsMoveUp = false;
			else
				newOrdinal--;
		}
		else
		{
			if (newOrdinal == 4)
			{
				newIsMoveUp = true;
				return;
			}
			else
				newOrdinal++;
		}
		//--end check move

		if (newIsSave)
			return;
		else
			newIsSave = false;
	} // end while
}

void InsertAfter(NODE_FLIGHT* p, FLIGHT &data)
{
	if (p == NULL) return;
	NODE_FLIGHT* q = new NODE_FLIGHT;
	q->data = data;
	q->pNext = p->pNext;
	p->pNext = q;
}

bool IsDeletedHead(LIST_FLIGHT &l)
{
	if (ListFlightIsEmpty(l)) return false;
	NODE_FLIGHT* p = l.pHead;
	l.pHead = p->pNext;
	delete p;
	--l.n;
	return true;
}

bool IsDeletedTail(LIST_FLIGHT &l)
{
	if (ListFlightIsEmpty(l)) return false;
		NODE_FLIGHT *beforeP = NULL;
		for (NODE_FLIGHT *p = l.pHead; p != NULL; p = p->pNext)
		{
			if (p == l.pTail)
			{
				l.pTail = beforeP;
				beforeP->pNext = NULL;
				delete p;
				--l.n;
				return true;
			}
			beforeP = p;
		}
			
}

bool IsDeletedAfter(LIST_FLIGHT &l, NODE_FLIGHT*p)
{
	if (p == NULL || p->pNext == NULL) return false;
	NODE_FLIGHT* nodeDeleted = p->pNext;
	p->pNext = nodeDeleted->pNext;
	delete nodeDeleted;
	--l.n;
	return true;
}
bool IsDeletedFlightWithId(LIST_FLIGHT &l, Flight data)
{
	NODE_FLIGHT* nodeDeleted = FindFlight(l, data.id);
	if (nodeDeleted == NULL) return false;
	if (nodeDeleted == l.pHead) return IsDeletedHead(l);
	if (nodeDeleted == l.pTail) return IsDeletedTail(l);
	else
	{
		NODE_FLIGHT* temp = l.pHead;
		while (temp->pNext != nodeDeleted)
			temp = temp->pNext;
		return IsDeletedAfter(l,temp);
	}
}

void deleteListFlight(LIST_FLIGHT &l)
{
	NODE_FLIGHT* temp = NULL;
	while (l.pHead != NULL)
	{
		temp = l.pHead;
		l.pHead = l.pHead->pNext;
		delete temp;
	}
} 
void AutoChangeSttFlight(LIST_FLIGHT &l)
{
	NODE_FLIGHT* p = l.pHead;
	for (p; p != NULL; p = p->pNext) {
		if (!DateTimeIsValid(p->data.dateLeave)) p->data.stt = STT_FINISHED;
	}
}

void SortIdFlight(int left, int right, char* a[]) {
	char* key = a[(left + right) / 2];
	int i = left, j = right;
	do {
		while (a[i] < key) i++;
		while (a[j] > key) j--;
		if (i <= j) {
			if (i < j) swap(a[i], a[j]);
			i++;	j--;
		}
	} while (i <= j);
	if (left < j) SortIdFlight(left, j, a);
	if (right > i) SortIdFlight(i, right, a);
}
void DisplayMenuTicket(FLIGHT fl, int day, int dong)
{
	int x = X_MENU_TICKET; int y = Y_MENU_TICKET;
	int n = fl.nTicket;
	for (int i = 1; i <= n; i++)
	{
		BoxTicket(x, y, fl.listTicket[i - 1].numberTicket, fl.listTicket[i - 1].stt);
		x += 6;
		if (i % day == 0)
		{
			y += 3;
			x = X_MENU_TICKET;
		}

	}
}

void DisplayMenuTicketEmpty(FLIGHT fl)
{
	int x = X_MENU_TICKET; int y = Y_MENU_TICKET;
	int n = fl.nTicket;
	for (int i = 1; i <= n; i++)
	{
		if (fl.listTicket[i - 1].stt == TICKET_EMPTY) {
			BoxTicket(x, y, fl.listTicket[i - 1].numberTicket, fl.listTicket[i - 1].stt);
			x += 6;
			if (i % 10 == 0)
			{
				y += 3;
				x = X_MENU_TICKET;
			}
		}
	}
}


void   EffectiveMenuTicket(int index, FLIGHT fl, int day)
{
	int preColor = fl.listTicket[currposPreTicket - 1].stt;
	int current = index;
	int column = (current - 1) % day;
	int row = (current - 1) / day;

	SetBGColor(6);
	Gotoxy(13 + (column + 1) * 6, (row + 1) * 3 + 1);
	cout << setw(3) << setfill(' ') << fl.listTicket[index - 1].numberTicket;

	column = (currposPreTicket - 1) % day;
	row = (currposPreTicket - 1) / day;

	if (preColor == 0) preColor = 5;
	SetBGColor(preColor);
	Gotoxy(13 + (column + 1) * 6, (row + 1) * 3 + 1);
	cout << setw(3) << setfill(' ') << fl.listTicket[currposPreTicket-1].numberTicket;
	currposPreTicket = current;
}

int ChooseTicket(FLIGHT fl, int day, int dong)
{
	Gotoxy(X_TITLE, Y_TITLE); cout << " CHON GHE MUON DAT";

	currposTicket = 1;
	currposPreTicket = 1;
	DisplayMenuTicket(fl,day,dong);
	int keyboard_read = 0;
	int PASS = 1;

	SetBGColor(6);
	Gotoxy(13 + (0 + 1) * 6, (0 + 1) * 3 + 1);
	cout << setw(3) << setfill(' ') << fl.listTicket[0].numberTicket;

	while (PASS)
	{
		keyboard_read = _getch();
		switch (keyboard_read)
		{
		case KEY_DOWN: //key down
			if (currposTicket + day <= fl.nTicket)
			{
				currposTicket = currposTicket + day;
				EffectiveMenuTicket(currposTicket, fl, day);
			}

			break;

		case KEY_UP: //key up
			if (currposTicket - day >= 1)
			{
				currposTicket = currposTicket - day;
				EffectiveMenuTicket(currposTicket, fl, day);
			}

			break;
		case KEY_LEFT: //left
			if (currposTicket > 1)
			{
				currposTicket--;
				EffectiveMenuTicket(currposTicket, fl, day);
			}
			break;
		case KEY_RIGHT: //right
			if (currposTicket < fl.nTicket)
			{
				currposTicket++;
				EffectiveMenuTicket(currposTicket, fl, day);
			}
			break;
		case ESC: return -1; break;

		case ENTER: //enter
			PASS = 0;
			return currposTicket;
			break;
		}//end switch keyboard
	}
	return 0;
}

void OutputFlight(FLIGHT fl, int locate)
{
	DeleteOldData(sizeof(keyDisplayFlight) / sizeof(string), locate);
	Gotoxy(xKeyDisplay[0] + 1, Y_DISPLAY + 3 + locate); cout << fl.id;
	Gotoxy(xKeyDisplay[1] + 1, Y_DISPLAY + 3 + locate); OutputDateTime(fl.dateLeave);
	Gotoxy(xKeyDisplay[2] + 1, Y_DISPLAY + 3 + locate); cout << fl.destiny;
	Gotoxy(xKeyDisplay[3] + 1, Y_DISPLAY + 3 + locate);
	switch (fl.stt)
	{
	case 0: cout << "Da Huy";
		break;
	case 1: cout << "Con ve";
		break;
	case 2: cout << "Het ve";
		break;
	case 3: cout << "Hoan tat";
		break;
	default:
		break;
	}
	Gotoxy(xKeyDisplay[4] + 1, Y_DISPLAY + 3 + locate); cout << fl.idMB;
}

void OutputFlightByDateAndDestiny(FLIGHT fl, int locate)
{
	DeleteOldData(sizeof(keyDisplayFlightOnDateAndPlace) / sizeof(string), locate);
	Gotoxy(xKeyDisplay[0] + 1, Y_DISPLAY + 3 + locate); cout << fl.id;
	Gotoxy(xKeyDisplay[1] + 1, Y_DISPLAY + 3 + locate); OutputDateTime(fl.dateLeave);
	Gotoxy(xKeyDisplay[2] + 1, Y_DISPLAY + 3 + locate); cout << fl.destiny;
	Gotoxy(xKeyDisplay[3] + 1, Y_DISPLAY + 3 + locate);
	switch (fl.stt)
	{
	case 0: cout << "Da Huy";
		break;
	case 1: cout << "Con ve";
		break;
	case 2: cout << "Het ve";
		break;
	case 3: cout << "Hoan tat";
		break;
	default:
		break;
	}
	Gotoxy(xKeyDisplay[4] + 1, Y_DISPLAY + 3 + locate); cout << fl.nTicket;
	Gotoxy(xKeyDisplay[5] + 1, Y_DISPLAY + 3 + locate); cout << fl.nTicket - fl.nTicketSold ;
}

void OutputListFlight(LIST_FLIGHT l)
{
	if (l.pHead == NULL && l.pTail == NULL) return;
	int count = -1;
	AutoChangeSttFlight(l);
	for (NODE_FLIGHT* q = l.pHead; q != NULL; q = q->pNext)
	{
		count++;
		OutputFlight(q->data, count);
	}
}
void OutputListFlightPerPage(LIST_FLIGHT l, int indexBegin)
{
	if (l.pHead == NULL && l.pTail == NULL) return;
	int count = -1;
	
	for (NODE_FLIGHT* q = l.pHead; q != NULL; q = q->pNext)
	{
		count++;
		if (count == indexBegin)
		{
			int i = -1; 
			while (q != NULL && i < QUANTITY_PER_PAGE - 1)
			{
				OutputFlight(q->data, (++i) * 2);
				q = q->pNext;
			}
			break;
		}
		
	}
	Gotoxy(X_PAGE, Y_PAGE);
	cout << "Trang " << pageNowFlight << "/" << totalPageFlight;
	return;

	
}

bool DataFlightIsEmpty(FLIGHT fl)
{
	if (strlen(fl.id) == 0) return true;
	if (strlen(fl.destiny) == 0) return true;
	if (strlen(fl.idMB) == 0) return true;
	if (fl.nTicket <= 0) return true;
	return false;
}

void InputFlight(LIST_FLIGHT &tempLF, LIST_FLIGHT &l, char* idMB, FLIGHT &fl, int soDay, int soDong, bool isEdited = false)
{
	ShowCur(true);
	int ordinal = 0;
	bool isMoveUp = false;
	bool isSave = false;
	bool idIsExist = false;
	string id;
	DateTime dt;
	string destiny;
	int stt = STT_REMAIN_TICKET, nTicket = soDay*soDong;

	Gotoxy(X_NOTIFY, Y_NOTIFY + 1);
	cout << "F10 luu ngay gio";
	Gotoxy(X_NOTIFY - 4, Y_NOTIFY + 2);
	cout << "Trang thai: 1:Con ve 2:Het ve 3:Hoan tat 0:Huy chuyen";

	if (isEdited)
	{
		//binding data
		id = fl.id;
		dt = fl.dateLeave;
		destiny = fl.destiny;
		stt = fl.stt;


		//format datetime
		Gotoxy(X_ADD + 12 + 2, 3 + Y_ADD);
		cout << ":";
		Gotoxy(X_ADD + 12 + 8, 3 + Y_ADD);
		cout << "/";
		Gotoxy(X_ADD + 12 + 11, 3 + Y_ADD);
		cout << "/";

		//out datetime
		Gotoxy(X_ADD + 12 + 0 * 3, 3 + Y_ADD);
		cout << dt.h;
		Gotoxy(X_ADD + 12 + 1 * 3, 3 + Y_ADD);
		cout << dt.mi;
		Gotoxy(X_ADD + 12 + 2 * 3, 3 + Y_ADD);
		cout << dt.d;
		Gotoxy(X_ADD + 12 + 3 * 3, 3 + Y_ADD);
		cout << dt.m;
		Gotoxy(X_ADD + 12 + 4 * 3, 3 + Y_ADD);
		cout << dt.y;

		//out data
		Gotoxy(X_ADD + 15, 0 * 3 + Y_ADD);
		cout << id;
		Gotoxy(X_ADD + 15, 2 * 3 + Y_ADD);
		cout << destiny;
		Gotoxy(X_ADD + 15, 3 * 3 + Y_ADD);
		cout << stt;
		Gotoxy(X_ADD + 15, 4 * 3 + Y_ADD);
		cout << idMB;
		
	}
	while (true)
	{
		Gotoxy(X_ADD + 15, 4 * 3 + Y_ADD);
		cout << idMB;
		//input
		switch (ordinal)
		{
		case 0:
		{
			if (isEdited) break;
			CheckMoveAndValidateID(id, isMoveUp, ordinal, isSave, 15);
			if (isEdited && _stricmp(id.c_str(), fl.id) == 0)  break;
			if (FindFlight(l, (char *)id.c_str()) == NULL)
			{
				idIsExist = false;
				break;
			}
			idIsExist = true;
			break;
		}
		case 1:
		{
			InputDatimeee(dt);
			break;
		}
		case 2:
		{
			CheckMoveAndValidateName(destiny, isMoveUp, ordinal, isSave, 15);
			break;
		}
		case 3:
		{
			CheckMoveAndValidateNumber(stt, isMoveUp, ordinal, isSave, 15, 1);
			break;
		}
		}//end switch

		 //--move
		if (isMoveUp)
		{
			if (ordinal == 0)
				isMoveUp = false; //block move up
			ordinal--;
		}
		else
		{
			if (ordinal == 4)
				isMoveUp = true; //block move down
			ordinal++;
		}
		//--end move

		if (isSave)
		{
			//binding data
			strcpy(fl.id, id.c_str());
			fl.dateLeave = dt;
			strcpy(fl.idMB, idMB);
			strcpy(fl.destiny, destiny.c_str());
			fl.stt = stt;
			fl.nTicket = soDay*soDong;
			if (!isEdited) {
				int indexTicket = 0;
				for (int i = 1; i <= soDong; i++) {
					for (int j = 1; j <= soDay; j++) {
						fl.listTicket[indexTicket].numberTicket = dayf[i-1] + dongf[j-1];
						++indexTicket;
					}
				}
			}
			Gotoxy(X_NOTIFY, Y_NOTIFY);
			cout << setw(50) << setfill(' ') << " ";
			if (!DateTimeIsValid(fl.dateLeave))
			{
				Gotoxy(X_NOTIFY, Y_NOTIFY);
				cout << "Ngay gio khong hop le. Phai lon hon gio he thong";

			}
			else if (DataFlightIsEmpty(fl))
			{
				Gotoxy(X_NOTIFY, Y_NOTIFY);
				cout << "Cac truong du lieu khong dc de trong";
			}
			else if (idIsExist)
			{
				Gotoxy(X_NOTIFY, Y_NOTIFY);
				cout << "Ma so khong duoc trung";
			}
			else
			{
				StandardName(fl.destiny);
				if (isEdited)
				{
					NODE_FLIGHT* p = FindFlight(l, fl.id);
					p->data = fl;
				}
				else
				{
					AddTailListFlight(l, fl);
				}
				totalPageFlight = ((l.n - 1) / QUANTITY_PER_PAGE) + 1;
				DeleteMenuAdd();
				return;
			}
			isSave = false;
		}
		else
			isSave = false;
	}
	
	ShowCur(false);
}
void SetDefaultChosenFlight(FLIGHT fl, int ordinal)
{
	SetBGColor(GREEN);
	OutputFlight(fl, (ordinal % QUANTITY_PER_PAGE) * 2);
	SetBGColor(PURPLE);
}
void   EffectiveMenuFlight(int ordinal, FLIGHT flNew, FLIGHT flOld)
{
	int current = ordinal;
	SetDefaultChosenFlight(flNew, current);

	/*SetBGColor(GREEN);
	OutputFlight(flNew, (current % QUANTITY_PER_PAGE) * 2);
	SetBGColor(PURPLE);*/
	OutputFlight(flOld, (currposPrecFlight % QUANTITY_PER_PAGE) * 2);
	currposPrecFlight = current;
}
LIST_FLIGHT findByIdMB(LIST_FLIGHT l, char* idMB) {
	LIST_FLIGHT lf;
	InitListFlight(lf);
	if (l.n > 0) {
		for (NODE_FLIGHT *k = l.pHead; k != NULL; k = k->pNext) {
			if (_stricmp(k->data.idMB, idMB) == 0) {
				AddTailListFlight(lf, k->data);
				//arrFlightId[++sttFlight] = k->data.id;
			}
		}
	}
	return lf;
}

LIST_FLIGHT findByIdDate(LIST_FLIGHT l, DATETIME date, string destiny) {
	LIST_FLIGHT lf;
	InitListFlight(lf);
	if (l.n > 0) {
		for (NODE_FLIGHT *k = l.pHead; k != NULL; k = k->pNext) {
			if (CompareDate(k->data.dateLeave,date) 
				&& _strcmpi(destiny.c_str(),k->data.destiny)==0
				&& (k->data.nTicketSold<k->data.nTicket)) {
				AddTailListFlight(lf, k->data);
			}
		}
	}
	return lf;
}
void ChangePageChooseFlight(LIST_FLIGHT l)
{

	system("cls");
	Display(keyDisplayFlight, sizeof(keyDisplayFlight) / sizeof(string));
	currposFlight = (pageNowFlight - 1) * QUANTITY_PER_PAGE;
	currposPrecFlight = (pageNowFlight - 1) * QUANTITY_PER_PAGE;
	
}

NODE_FLIGHT* ChooseFlight(LIST_FLIGHT l)
{
	backMenu:
	int keyboard_read = 0;
	ShowCur(false);
	pageNowFlight = 1;
	currposFlight = (pageNowFlight - 1) * QUANTITY_PER_PAGE;
	currposPrecFlight = (pageNowFlight - 1) * QUANTITY_PER_PAGE;
	totalPageFlight = ((l.n-1) / QUANTITY_PER_PAGE) + 1;
	
	NODE_FLIGHT* newNodeFlight = FindFlightByOridnal(l, currposFlight);
	NODE_FLIGHT* oldNodeFilght = NULL;
	
	OutputListFlightPerPage(l, (pageNowFlight - 1) * QUANTITY_PER_PAGE);
	SetDefaultChosenFlight(newNodeFlight->data, currposFlight);
	while (true)
	{
		keyboard_read = _getch();
		if (keyboard_read == 0)
			keyboard_read = _getch();
		switch (keyboard_read)
		{
		case KEY_UP:
			if (currposFlight % QUANTITY_PER_PAGE> 0)
			{
				currposFlight = currposFlight - 1;
				oldNodeFilght = newNodeFlight;
				for (newNodeFlight = l.pHead; newNodeFlight->pNext != oldNodeFilght; newNodeFlight = newNodeFlight->pNext);
				EffectiveMenuFlight(currposFlight, newNodeFlight->data, oldNodeFilght->data);
			}
			break;
		case KEY_DOWN:
			if (currposFlight  % QUANTITY_PER_PAGE < QUANTITY_PER_PAGE - 1 && newNodeFlight->pNext != NULL) //&& currposFlight < 3)
			{
				currposFlight = currposFlight + 1;
				oldNodeFilght = newNodeFlight;
				newNodeFlight = newNodeFlight->pNext;
				EffectiveMenuFlight(currposFlight, newNodeFlight->data, oldNodeFilght->data);
			}
			break;
		
		case PAGE_DOWN:
			if (pageNowFlight < totalPageFlight)
			{

				pageNowFlight++;
				ChangePageChooseFlight(l);
				OutputListFlightPerPage(l, (pageNowFlight - 1) * QUANTITY_PER_PAGE);
				newNodeFlight = FindFlightByOridnal(l, currposFlight);
				SetDefaultChosenFlight(newNodeFlight->data, currposFlight);
				
			}
			break;
		case PAGE_UP:
			if (pageNowFlight > 1)
			{
			
				pageNowFlight--;	
				ChangePageChooseFlight(l);
				OutputListFlightPerPage(l, (pageNowFlight - 1) * QUANTITY_PER_PAGE);
				newNodeFlight = FindFlightByOridnal(l, currposFlight);
				SetDefaultChosenFlight(newNodeFlight->data, currposFlight);
				
			}
			break;


		case ENTER:  //enter
			ShowCur(true);
			return newNodeFlight;
			break;
		case ESC: 
			goto backMenu;
		}//end switch  read key
	}
}

void ChangePageManageFlight(LIST_FLIGHT l)
{
	system("cls");
	Gotoxy(X_TITLE, Y_TITLE); cout << "QUAN LY CAC CHUYEN BAY";
	OutputListFlightPerPage(l, (pageNowFlight - 1) * QUANTITY_PER_PAGE);
	Display(keyDisplayFlight, sizeof(keyDisplayFlight) / sizeof(string));
}
void MenuManageFlight(char* idMB, int soDay, int soDong, LIST_FLIGHT &l, LIST_FLIGHT &tempLF, TREE_PASSENGER &p)
{

backMenu:
	system("cls");
	pageNowFlight = 1;
//	SortIdFlight(0, sttFlight, arrFlightId);
	OutputListFlightPerPage(tempLF, 0);
	Display(keyDisplayFlight, sizeof(keyDisplayFlight) / sizeof(string));
	DeleteNote(5);
	Gotoxy(xKeyDisplay[0] + 1, Y_DISPLAY + 40);
	cout << setw(5) << "F2: them F3: Xoa F4: Hieu Chinh F5: Huy Chuyen F10: Save" << setfill(' ');
	Gotoxy(X_TITLE, Y_TITLE); cout << "QUAN LY CAC CHUYEN BAY";
	int key;
	while (true)
	{
		while (_kbhit())
		{
			key = _getch();
			if (key == 0 || key == 224)
			{
				key = _getch();

				if (key == KEY_F2) //f2
				{
					FLIGHT fl;
					DisplayEdit(keyDisplayFlight, sizeof(keyDisplayFlight) / sizeof(string), 35);
					InputFlight(tempLF, l, idMB, fl, soDay, soDong, false);
					tempLF = findByIdMB(l, idMB);
					totalPageFlight = getTotalPage(tempLF.n);
					ChangePageManageFlight(tempLF);
					Gotoxy(X_NOTIFY, Y_NOTIFY);
					cout << "Them thanh cong";
				}
				else if (key == KEY_F3) //f3
				{
					if (tempLF.n == 0) {
						goto backMenu;
					}
					NODE_FLIGHT* k = ChooseFlight(tempLF);
					Gotoxy(X_NOTIFY, Y_NOTIFY);
					cout << "Ban co chac chan xoa? Enter dong y";
					key = _getch();
					if (key == ENTER)
					{
						if (k->data.nTicketSold != 0)
						{
							Gotoxy(X_NOTIFY, Y_NOTIFY);
							cout << "Khong xoa duoc vi da co nguoi dat ve";
							_getch();
							goto backMenu;
						}
						if (IsDeletedFlightWithId(l, k->data))
						{

							
							//pageNowFlight = 1;
							tempLF = findByIdMB(l, idMB);
							if (tempLF.n % QUANTITY_PER_PAGE == 0) pageNowFlight--;
							totalPageFlight = getTotalPage(tempLF.n);
							ChangePageManageFlight(tempLF);
							Gotoxy(X_NOTIFY, Y_NOTIFY);
							cout << "Xoa thanh cong";
						}
					}
					else
						goto backMenu;
				}
				else if (key == KEY_F4) //f4
				{
					if (tempLF.n == 0) {
						goto backMenu;
					}
					NODE_FLIGHT* k = ChooseFlight(tempLF);
					DisplayEdit(keyDisplayFlight, sizeof(keyDisplayFlight) / sizeof(string), 35);
					InputFlight(tempLF, l, idMB, k->data, soDay, soDong, true);
					tempLF = findByIdMB(l, idMB);
					ChangePageManageFlight(tempLF);
					Gotoxy(X_NOTIFY, Y_NOTIFY);
					cout << "Sua thanh cong";
				}
				else if (key == KEY_F5) //f5
				{
					DeleteNotify();
					Gotoxy(X_NOTIFY, Y_NOTIFY);
					cout << "Nhan ENTER de huy chuyen bay";
					if (tempLF.n == 0) {
						goto backMenu;
					}
					NODE_FLIGHT* k = ChooseFlight(tempLF);
					if (k->data.stt == 3) {
						DeleteNotify();
						Gotoxy(X_NOTIFY, Y_NOTIFY);
						cout << "Khong The Huy Chuyen Da Hoan Tat";
					}
					else {
						NODE_FLIGHT* kSave = FindFlight(l, k->data.id);
						kSave->data.stt = 0;
						for (int i = 0; i < kSave->data.nTicket; i++) {
							if (kSave->data.listTicket[i].stt == TICKET_SOLD) {
								FindPassenger(p, kSave->data.listTicket[i].idOwner)->data.isBooked--;
								kSave->data.listTicket[i].stt = 0;
								kSave->data.listTicket[i].idOwner = 0;
							}
						}
						tempLF = findByIdMB(l, idMB);
						ChangePageManageFlight(tempLF);
						DeleteNotify();
						Gotoxy(X_NOTIFY, Y_NOTIFY);
						cout << "Huy thanh cong";
					}
					
				}
				else if (key == PAGE_DOWN && pageNowFlight < totalPageFlight)
				{
					pageNowFlight++;
					ChangePageManageFlight(tempLF);

				}
				else if (key == PAGE_UP && pageNowFlight > 1)
				{
					pageNowFlight--;
					ChangePageManageFlight(tempLF);

				}
			}
			else if (key == ESC)
				return;
		}//end while kbhit
	}
}
#endif