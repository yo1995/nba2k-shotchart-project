#include "trainerbase.h"

#include "minicsv.h"

/*
���ļ���������ѭ����ȡ�ڴ��Ը�����ʾֵ
*/

// address table. might not be universal.
// will test after all wrapedup.


// 0. the followings are for all courts. 
DWORD exe_base_addr				= 0x00400000;	// nba2k11.exe��ַ����֪���᲻��仯������ĵ�ַ���ǻ�ַ+ƫ������ĵ�ַ��
// e.g. single practice, blacktop street, shooting practice, etc. 
DWORD score_type_addr			= 0x05D5FA70;	//  ���������ϵ��0����մ1�ҿ�2��ˢ3��� 5��ſ�ˢ/ƫ���� 7����ҿ��/�Ұ�����8�������������Բ����
DWORD score_judge_addr			= 0x05D5FA78;	//  �����ж�
DWORD coordinate_x_100_addr		= 0x05d5f728;	//  �ٱ������� - ��˵�ӽ�����ϵ
DWORD coordinate_y_100_addr		= 0x05d5f720;	//  �ٱ�������
DWORD absolute_dist_rim_addr	= 0x05d5f730;	//  ����������Ծ���
DWORD projected_percent_addr	= 0x05d5f738;	// Ԥ��Ͷ�������ʣ��ɲο�������Ͷ��ѡ��
DWORD shot_triggered_time_addr	= 0x05db028c;	// Ͷ���ͷ�ʱ�䣬������zͶ�������̧���ʱ�䣬�������ж��Ƿ����������
DWORD total_time_elapsed_addr1	= 0x05439C1C;	// ������ʱ�䣬������ͣ���ݵȣ�������esc��Ϸ��ͣ

// 1. the followings are for quartered games, e.g.
// dynasty, quick game, mplayer, jordan, etc.
DWORD game_time_elapsed_addr	= 0x05544DF4;	// ��������ʱ�䣬�������ڷֽڱ�����ģʽ����������ʱ����


// 2. the following data are for SG player, particular for jordan mplayer mode
DWORD FTA_ADDR1 = 0x0;
DWORD FGA_ADDR1 = 0x0;
DWORD PA3_ADDR1 = 0x0;

// 180808 change score type method
DWORD FTM_ADDR1 = 0x0;
DWORD FGM_ADDR1 = 0x0;
DWORD PM3_ADDR1 = 0x0;
int FGM_1 = 0;
int FTM_1 = 0;
int PM3_1 = 0;

DWORD HOME_PG_BASE_ADDR = 0x05c2e14f;
DWORD NAME_STRUCT_OFFSET = 0x11;
DWORD PTS_OFFSET = 0x19;
DWORD PLAYER_OFFSET = 0x43C;

DWORD HOME_PG_SUR_ADDR = 0x5e4a528;
DWORD HOME_PG_FIR_ADDR = 0x5e4a52c;  // 4bytes for last name(SUR) pointer
DWORD PLAYERS_INFO_OFFSET = 0xB58;  // pg sg sf pf c
DWORD DIFF_FIR_SUR_ADDR = 0x4;  // 4bytes between pointers

void update_attempts_addresses() {
	FTA_ADDR1 = PTS_ADDR + 0x8;
	FGA_ADDR1 = PTS_ADDR + 0x10;
	PA3_ADDR1 = PTS_ADDR + 0x18;
}

void update_made_addresses() {
	FTM_ADDR1 = PTS_ADDR + 0x4;
	FGM_ADDR1 = PTS_ADDR + 0xC;
	PM3_ADDR1 = PTS_ADDR + 0x14;
}

bool if_game_started(HANDLE pHandle) {
	float total_time_elapsed_temp;
	ReadProcessMemory(pHandle, (LPVOID)total_time_elapsed_addr1, &total_time_elapsed_temp, sizeof(total_time_elapsed_temp), 0);
	return total_time_elapsed_temp != total_time_elapsed ? true : false;
}

int acquire_PTS_ADDR(HANDLE pHandle) {
	int i = 0;
	DWORD ptSTAT_ADDR = 0x0;
	DWORD ptSUR_ADDR = 0x0;  // the addr of pointer that points to 
	DWORD ptFIR_ADDR = 0x0;
	DWORD SUR_ADDR = 0x0;
	DWORD FIR_ADDR = 0x0;
	wchar_t SUR[7] = L"Jordan";  // Jordan in unicode string
	wchar_t FIR[8] = L"Michael";  // Michael
	while(i < 24) {
		// first, loop through all the player stat entries
		
		ptSTAT_ADDR = HOME_PG_BASE_ADDR + i * PLAYER_OFFSET;
		ptSTAT_ADDR += NAME_STRUCT_OFFSET;  // offset pointer to 0x11 of STAT STRUCT
		ReadProcessMemory(pHandle, (LPVOID)ptSTAT_ADDR, &ptSUR_ADDR, sizeof(ptSUR_ADDR), 0);
		ptFIR_ADDR = DIFF_FIR_SUR_ADDR + ptSUR_ADDR;


		// ptSUR_ADDR = HOME_PG_SUR_ADDR + i * PLAYERS_INFO_OFFSET;
		// ptFIR_ADDR = HOME_PG_FIR_ADDR + i * PLAYERS_INFO_OFFSET;

		// second, read player name from both addresses

		ReadProcessMemory(pHandle, (LPVOID)ptSUR_ADDR, &SUR_ADDR, sizeof(SUR_ADDR), 0);
		ReadProcessMemory(pHandle, (LPVOID)ptFIR_ADDR, &FIR_ADDR, sizeof(FIR_ADDR), 0);
		wchar_t SUR_BUF[64];  // unicode string, make bigger space to avoid overflow
		wchar_t FIR_BUF[64];  //
		ReadProcessMemory(pHandle, (LPVOID)SUR_ADDR, &SUR_BUF, sizeof(SUR_BUF), 0);
		ReadProcessMemory(pHandle, (LPVOID)FIR_ADDR, &FIR_BUF, sizeof(FIR_BUF), 0);

		// third, if name is michael jordan, break and set PTS_ADDR
		if (VERBOSEMODE) MessageBoxW(0, SUR_BUF, L"see what surname we got", 0);
		if ((wcsncmp(SUR, SUR_BUF, 7) == 0) && (wcsncmp(FIR, FIR_BUF, 8) == 0)) {
			if (VERBOSEMODE) MessageBox(0, "haha we caught Jordan!", "nana...", 0);
			PTS_ADDR = HOME_PG_BASE_ADDR + PTS_OFFSET + i * PLAYER_OFFSET;
			return true;
		}
		++i;
	}
	// return i;  // i = 24 fail, otherwise find offset factor
	return false;  // if not break in the loop, then failed.
}


// we can either output a string vector or a single string containing team name.
std::wstring acquire_home_team(HANDLE pHandle) {
	DWORD ROSTER_OFFSET = 0x14;  // where the whole roster list lies
	DWORD TEAM_NAME_OFFSET = 0xA0;  // where the team name lies

	DWORD ptSUR = HOME_PG_SUR_ADDR + ROSTER_OFFSET;
	DWORD SUR_ADDR = 0x0;
	DWORD ROSTER_ADDR = 0x0;

	// first, home team
	ReadProcessMemory(pHandle, (LPVOID)ptSUR, &ROSTER_ADDR, sizeof(ROSTER_ADDR), 0);  // first read pointer to home pg surname
	ROSTER_ADDR += TEAM_NAME_OFFSET;
	wchar_t TEAM_NAME_BUF[64];
	wchar_t TEAM_CITY_BUF[64];
	DWORD ptString_temp = 0x0;

	ReadProcessMemory(pHandle, (LPVOID)ROSTER_ADDR, &ptString_temp, sizeof(ptString_temp), 0);
	ReadProcessMemory(pHandle, (LPVOID)ptString_temp, &TEAM_NAME_BUF, sizeof(TEAM_NAME_BUF), 0);  // third read pointer to roster team name
	ReadProcessMemory(pHandle, (LPVOID)(ROSTER_ADDR + 0x8), &ptString_temp, sizeof(ptString_temp), 0);
	ReadProcessMemory(pHandle, (LPVOID)ptString_temp, &TEAM_CITY_BUF, sizeof(TEAM_CITY_BUF), 0);  // third read pointer to roster team city
	if (VERBOSEMODE) MessageBoxW(0, TEAM_NAME_BUF, L"see what home team we got", 0);
	return std::wstring(TEAM_NAME_BUF);
}

std::wstring acquire_away_team(HANDLE pHandle) {
	DWORD ROSTER_OFFSET = 0x14;  // where the whole roster list lies
	DWORD TEAM_NAME_OFFSET = 0xA0;  // where the team name lies

	DWORD ptSUR = HOME_PG_SUR_ADDR + ROSTER_OFFSET + 12 * PLAYERS_INFO_OFFSET;
	DWORD SUR_ADDR = 0x0;
	DWORD ROSTER_ADDR = 0x0;

	// first, home team
	ReadProcessMemory(pHandle, (LPVOID)ptSUR, &ROSTER_ADDR, sizeof(ROSTER_ADDR), 0);  // first read pointer to away pg surname,
	ROSTER_ADDR = ROSTER_ADDR + TEAM_NAME_OFFSET;
	wchar_t TEAM_NAME_BUF[64];
	wchar_t TEAM_CITY_BUF[64];
	DWORD ptString_temp = 0x0;

	ReadProcessMemory(pHandle, (LPVOID)ROSTER_ADDR, &ptString_temp, sizeof(ptString_temp), 0);
	ReadProcessMemory(pHandle, (LPVOID)ptString_temp, &TEAM_NAME_BUF, sizeof(TEAM_NAME_BUF), 0);  // third read pointer to roster team name
	ReadProcessMemory(pHandle, (LPVOID)(ROSTER_ADDR + 0x8), &ptString_temp, sizeof(ptString_temp), 0);
	ReadProcessMemory(pHandle, (LPVOID)ptString_temp, &TEAM_CITY_BUF, sizeof(TEAM_CITY_BUF), 0);  // third read pointer to roster team city
	if (VERBOSEMODE) MessageBoxW(0, TEAM_NAME_BUF, L"see what away team we got", 0);
	return std::wstring(TEAM_NAME_BUF);
}

void update_shot_coordinates(HANDLE pHandle) {
	// update_shot_coordinates
	ReadProcessMemory(pHandle, (LPVOID)coordinate_x_100_addr, &coordinate_x_100, sizeof(coordinate_x_100), 0);
	ReadProcessMemory(pHandle, (LPVOID)coordinate_y_100_addr, &coordinate_y_100, sizeof(coordinate_y_100), 0);
	ReadProcessMemory(pHandle, (LPVOID)absolute_dist_rim_addr, &rim_dist, sizeof(rim_dist), 0);
}


void update_shot_triggered_time(HANDLE pHandle) {
	ReadProcessMemory(pHandle, (LPVOID)shot_triggered_time_addr, &shot_triggered_time, sizeof(shot_triggered_time), 0);
}

void update_score_type(HANDLE pHandle) {
	ReadProcessMemory(pHandle, (LPVOID)score_type_addr, &score_type, sizeof(score_type), 0);
}

void update_score_judge_2(HANDLE pHandle) {
	// 180808 after some testing I found this addr is not fully reliable
	// corner case occurs in cases like shot blocked after foul, goaltending, off. intf., etc.
	// hence I switch to read FGM 3PM FTM.
	ReadProcessMemory(pHandle, (LPVOID)score_judge_addr, &score_judge, sizeof(score_judge), 0);
}

void update_score_judge_1(HANDLE pHandle) {
	int FGM_temp = FGM_1;
	int FTM_temp = FTM_1;
	int PM3_temp = PM3_1;
	ReadProcessMemory(pHandle, (LPVOID)FGM_ADDR1, &FGM_1, sizeof(FGM_1), 0);
	ReadProcessMemory(pHandle, (LPVOID)FTM_ADDR1, &FTM_1, sizeof(FTM_1), 0);
	ReadProcessMemory(pHandle, (LPVOID)PM3_ADDR1, &PM3_1, sizeof(PM3_1), 0);
	((FGM_temp != FGM_1) || (FTM_temp != FTM_1) || (PM3_temp != PM3_1)) ? score_judge_jordan = 1: score_judge_jordan = 0;
}

void update_projected_percent(HANDLE pHandle) {
	// move to above function to improve performance
	// update_projected_percent
	ReadProcessMemory(pHandle, (LPVOID)projected_percent_addr, &projected_percent, sizeof(projected_percent), 0);
}


// aka direct memory access. manipulate the memory region.
// only read from global flags and the handle for address
void UpdateDMAs(HANDLE pHandle, SaveData *mSaveData) {
	if (if_game_started(pHandle)) {  // game started
		update_score_type(pHandle);
		update_score_judge_2(pHandle);  // in game display
		update_projected_percent(pHandle);

		float shot_time_temp = shot_triggered_time;
		float coordinate_x_temp = coordinate_x_100;
		update_shot_triggered_time(pHandle);
		if (shot_time_temp != shot_triggered_time) {  // anybody make a shot, ai, bot, teammate, etc.
			update_shot_coordinates(pHandle);
			(coordinate_x_temp == coordinate_x_100) ? (is_a_dunk = true) : (is_a_dunk = false);	 // the tricy part ;-)

			if (record_mode == 2) {
				if (made_shot_Z_down) {
					// then myself triggered a shot. update the x-ys
					// alley-oops are ignored due to my noobieness. :-<
					if (record_shot_chart_and_more) {
						update_score_type(pHandle);  
						update_score_judge_2(pHandle);
						mSaveData->SaveDataFileLines();  // only record when toggled to true
					}
					redraw_shotchart = true;	// serve as a lock to control the read from vector
					made_shot_Z_down = false;	// reset the state and wait for another shot
				}
				else {
					redraw_shotchart = false;
				}
			}
		}


		if (record_mode == 1 && record_shot_chart_and_more) {  // mj mp mode
			if (!PTS_ADDR) {  // addr == 0
				bool success_flag;
				success_flag = acquire_PTS_ADDR(pHandle);
				mSaveData->home = acquire_home_team(pHandle);
				mSaveData->away = acquire_away_team(pHandle);
				if (!success_flag) {  // looped through all the players and cannot find jordan
					MessageBoxW(0, L"MP addr not found error.", L"Error!", 0);
					return;  // if mj mode addr not initialized, do not record.
				}
				else {
					// PTS_ADDR = HOME_PG_BASE_ADDR + PTS_OFFSET + index * PLAYER_OFFSET;
					update_attempts_addresses();
					update_made_addresses();
					return;
				}
			}
			else {
				int fgatemp = fga_global;
				int pa3temp = pa3_global;
				int ftatemp = fta_global;
				ReadProcessMemory(pHandle, (LPVOID)FGA_ADDR1, &fga_global, sizeof(fga_global), 0);
				ReadProcessMemory(pHandle, (LPVOID)PA3_ADDR1, &pa3_global, sizeof(pa3_global), 0);
				ReadProcessMemory(pHandle, (LPVOID)FTA_ADDR1, &fta_global, sizeof(fta_global), 0);
				// read the points scored out and print it to see if addresses are correct.
				ReadProcessMemory(pHandle, (LPVOID)PTS_ADDR, &PTS, sizeof(PTS), 0);
				update_score_type(pHandle);  // update again to ensure status are correct
				update_score_judge_2(pHandle);
				update_score_judge_1(pHandle);
				if (fgatemp != fga_global) {
					pts_type = 2;
					redraw_shotchart = true;
					mSaveData->SaveDataFileLines();
				}
				else if (pa3temp != pa3_global) {
					pts_type = 3;
					redraw_shotchart = true;
					mSaveData->SaveDataFileLines();
				}
				else if (ftatemp != fta_global) {
					pts_type = 1;
					redraw_shotchart = true;
					mSaveData->SaveDataFileLines();
				}
				else {
					pts_type = 0;  // default
					redraw_shotchart = false;
				}
			}

		}
	}
	else {
		return;  // game not started
	}
}