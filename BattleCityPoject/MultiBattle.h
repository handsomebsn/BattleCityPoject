#pragma once
#ifndef __MultiBattle__
#include"HEAD.h"
//��Ϸ��ʼ
void Start_2()
{
	SetTimer(hwnd, GAME_ID, GAME_TIME, NULL);
	player_tank = BaseTank(20 / 10 - 1, 20, 0, UP, 5 * 64, 10 * 64 + 64, UP, false, 16, 4, 8, 2, 1, 10, 8);
	game_state = GAME;
	if (start_flag == true) {
		Init_2();
	}
}
//��ʼ��
void Init_2()
{
	MessageBoxA(hwnd, "��ս��ʼ", "", MB_OK);
	//�洢�ؿ��͵÷�
/*	MYSQL mysql;
	mysql_init(&mysql);
	if (mysql_real_connect(&mysql, db_host, db_user, db_passwd, db_name, db_port, NULL, 0))
	{
		char *buffer = (char*)malloc(sizeof(char) * 128);
		sprintf(buffer, "UPDATE `battleworld`.`account` SET `gamestage`='%d', `grade`='%d'  WHERE `email`='%s';", stage + 1, grade + grade_db, username);
		mysql_query(&mysql, buffer);
	}
	mysql_close(&mysql);*/

	//BOSSģʽ
	boss_mode = false;
	//���̹��
	if (cLientORServer == 1) {
		player_tank = BaseTank(
			player_tank.id,
			player_tank.life,
			player_tank.armor,
			UP,
			5 * 64,
			10 * 64 + 64,
			UP,
			player_tank.gun_lock,
			64 / player_tank.speed,
			player_tank.speed,
			player_tank.fire_speed,
			player_tank.bullet_id,
			player_tank.bullet_max,
			player_tank.bullet_power,
			player_tank.bullet_speed);
	}
	else if(cLientORServer==2)
	{
		player_tank = BaseTank(
			player_tank.id,
			player_tank.life,
			player_tank.armor,
			DOWN,
			9 * 64,
			-1 * 64 + 0,
			DOWN,
			player_tank.gun_lock,
			64 / player_tank.speed,
			player_tank.speed,
			player_tank.fire_speed,
			player_tank.bullet_id,
			player_tank.bullet_max,
			player_tank.bullet_power,
			player_tank.bullet_speed);
	}
	//����Ϊ��̬��Դ���ͷ�
	//����ӵ����
	for (list<Bullet*>::iterator i = player_bullet.begin(); i != player_bullet.end(); ++i)
		delete *i;
	player_bullet.clear();
	//���������
	for (list<Fire*>::iterator i = fire.begin(); i != fire.end(); ++i)
		delete *i;
	fire.clear();
	//����
	base_time = 0;
	//��һָ�
	player_death = false;
	player_time = 0;
	//ש���ˮѭ����ʱ����
	wall_time = 0;
	water_time = 0;
	//��������
	item = NONE;
	invin_time = 0;
	LoadMap();
}
//��Ϸ����
void Game_2()
{
	//��Ҳ���������Ӧ
	if (player_death == false)
		Key();
	//������Ϸ����
	DrawGame();
	//���ػ���
	if (base_time > 0) {
		if (--base_time == 0)
			game_state = GAMEOVER;
	}
	//�������
	if (player_death == true)
	{
		if (--player_time == 0)
		{
			if (player_num == 0)
				game_state = GAMEOVER;
			else {
				//��Ҹ���
				player_death = false;
				player_tank = BaseTank(20 / 10 - 1, 20, 0, UP, 5 * 64, 10 * 64 + 64, UP, false, 16, 4, 8, 2, 1, 10, 8);
			}
			InvalidateRect(hwnd, NULL, false);
		}
	}
}
//��Ϸ��λ����
void DrawGame_2()
{
	//���Ʊ���
	DrawBG();
	//���Ƶ�ͼ��
	DrawBlock();
	//����ͻ��Ƶ���
	Item();
	//����ͻ������̹��
	if (player_death == false)
		player_tank.Draw(writeDC, player_tankDC, false);
	//���Ʋݿ�
	DrawGrass();
	//����ͻ�������ڵ�
	PlayerBullet();
	//����ͻ��ƿ�������
	player_tank.Draw(writeDC, player_tankDC, true);
	//���ƻ�
	DrawFire();
	//�������Ļ
	Print();
}

#endif