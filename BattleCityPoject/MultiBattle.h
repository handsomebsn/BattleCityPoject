#pragma once
#ifndef __MultiBattle__
#include"HEAD.h"
//游戏开始
void Start_2()
{
	SetTimer(hwnd, GAME_ID, GAME_TIME, NULL);
	player_tank = BaseTank(20 / 10 - 1, 20, 0, UP, 5 * 64, 10 * 64 + 64, UP, false, 16, 4, 8, 2, 1, 10, 8);
	game_state = GAME;
	if (start_flag == true) {
		Init_2();
	}
}
//初始化
void Init_2()
{
	MessageBoxA(hwnd, "对战开始", "", MB_OK);
	//存储关卡和得分
/*	MYSQL mysql;
	mysql_init(&mysql);
	if (mysql_real_connect(&mysql, db_host, db_user, db_passwd, db_name, db_port, NULL, 0))
	{
		char *buffer = (char*)malloc(sizeof(char) * 128);
		sprintf(buffer, "UPDATE `battleworld`.`account` SET `gamestage`='%d', `grade`='%d'  WHERE `email`='%s';", stage + 1, grade + grade_db, username);
		mysql_query(&mysql, buffer);
	}
	mysql_close(&mysql);*/

	//BOSS模式
	boss_mode = false;
	//玩家坦克
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
	//以下为动态资源的释放
	//玩家子弹清空
	for (list<Bullet*>::iterator i = player_bullet.begin(); i != player_bullet.end(); ++i)
		delete *i;
	player_bullet.clear();
	//火花容器清空
	for (list<Fire*>::iterator i = fire.begin(); i != fire.end(); ++i)
		delete *i;
	fire.clear();
	//基地
	base_time = 0;
	//玩家恢复
	player_death = false;
	player_time = 0;
	//砖块和水循环延时清零
	wall_time = 0;
	water_time = 0;
	//道具清零
	item = NONE;
	invin_time = 0;
	LoadMap();
}
//游戏过程
void Game_2()
{
	//玩家操作按键响应
	if (player_death == false)
		Key();
	//绘制游戏画面
	DrawGame();
	//基地毁灭
	if (base_time > 0) {
		if (--base_time == 0)
			game_state = GAMEOVER;
	}
	//玩家死亡
	if (player_death == true)
	{
		if (--player_time == 0)
		{
			if (player_num == 0)
				game_state = GAMEOVER;
			else {
				//玩家复活
				player_death = false;
				player_tank = BaseTank(20 / 10 - 1, 20, 0, UP, 5 * 64, 10 * 64 + 64, UP, false, 16, 4, 8, 2, 1, 10, 8);
			}
			InvalidateRect(hwnd, NULL, false);
		}
	}
}
//游戏单位绘制
void DrawGame_2()
{
	//绘制背景
	DrawBG();
	//绘制地图块
	DrawBlock();
	//处理和绘制道具
	Item();
	//处理和绘制玩家坦克
	if (player_death == false)
		player_tank.Draw(writeDC, player_tankDC, false);
	//绘制草块
	DrawGrass();
	//处理和绘制玩家炮弹
	PlayerBullet();
	//处理和绘制开火闪光
	player_tank.Draw(writeDC, player_tankDC, true);
	//绘制火花
	DrawFire();
	//输出到屏幕
	Print();
}

#endif