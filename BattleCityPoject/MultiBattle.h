#pragma once
#ifndef __MultiBattle__
#include"HEAD.h"
bool load_enemytank = true;
//游戏开始
void Start_2()
{
	SetTimer(hwnd, GAME_ID, GAME_TIME, NULL);
	player_tank = BaseTank(20 / 10 - 1, 20, 0, UP, 5 * 64, 10 * 64 + 64, UP, false, 16, 4, 8, 2, 1, 10, 8);
	enemy2_tank=BaseTank(GOLD, 20, 0, DOWN, 9 * 64, -64 , DOWN, false, 16, 4, 8, 2, 1, 10, 8);
	game_state = GAME;
	if (start_flag == true) {
		Init_2();
	}
}
//初始化
void Init_2()
{
	MessageBoxA(hwnd, "对战开始", "", MB_OK);
	stage = 0;
	//BOSS模式
	boss_mode = false;
	//准许加载敌方坦克
	load_enemytank = true;
	//玩家坦克
	if (clientORserver == 1) {
		player_tank = BaseTank(
			player_tank.id,	player_tank.life,player_tank.armor,
			UP,	5 * 64,	10 * 64 + 64,UP,
			player_tank.gun_lock,
			64 / player_tank.speed,	player_tank.speed,
			player_tank.fire_speed,
			player_tank.bullet_id, player_tank.bullet_max,
			player_tank.bullet_power, player_tank.bullet_speed);
		enemy2_tank = BaseTank(
			GOLD, player_tank.life, player_tank.armor,
			DOWN, 9 * 64, -64, DOWN,
			player_tank.gun_lock,
			64 / player_tank.speed, player_tank.speed,
			player_tank.fire_speed,
			player_tank.bullet_id, player_tank.bullet_max,
			player_tank.bullet_power, player_tank.bullet_speed);
	}else if(clientORserver==2)
	{
		player_tank = BaseTank(
			player_tank.id,	player_tank.life, player_tank.armor,
			DOWN, 9 * 64, -64 ,	DOWN,
			player_tank.gun_lock,
			64 / player_tank.speed,	player_tank.speed,
			player_tank.fire_speed,
			player_tank.bullet_id,	player_tank.bullet_max,
			player_tank.bullet_power, player_tank.bullet_speed);
		enemy2_tank = BaseTank(
			GOLD, player_tank.life, player_tank.armor,
			UP, 5 * 64, 10 * 64 + 64, UP,
			player_tank.gun_lock,
			64 / player_tank.speed, player_tank.speed,
			player_tank.fire_speed,
			player_tank.bullet_id, player_tank.bullet_max,
			player_tank.bullet_power, player_tank.bullet_speed);
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
	//敌人容器清空
	for (list<BaseTank*>::iterator i = enemy_tank.begin(); i != enemy_tank.end(); ++i)
		delete *i;
	enemy_tank.clear();
	//敌人炮弹容器清空
	for (list<list<Bullet*>*>::iterator i = enemy_bullet.begin(); i != enemy_bullet.end(); ++i)
	{
		for (list<Bullet*>::iterator j = (**i).begin(); j != (**i).end(); ++j)
			delete *j;
		(**i).clear();
		delete *i;
	}
	enemy_bullet.clear();
	//关卡
	//wintime = 0;
	//基地
	base_time = 0;
	//玩家恢复
	player_death = false;
	player_time = 0;
	//敌人数量
	enemy_num = 0;
	enemy_total = 1;
	enemy_rest = enemy_total;
	enemy_time = 1;
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
	DrawGame_2();
	//基地毁灭
	if (base_time > 0) {
		if (--base_time == 0)
			game_state = GAMEOVER;
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
	//Item();
	//处理和绘制敌人坦克
	EnemyTank_2();
	//处理和绘制玩家坦克
	if (player_death == false)
		player_tank.Draw(writeDC, player_tankDC, false);
	//绘制草块
	DrawGrass();
	//处理和绘制敌人炮弹
	//EnemyBullet();
	//处理和绘制玩家炮弹
	PlayerBullet();
	//处理和绘制开火闪光
	player_tank.Draw(writeDC, player_tankDC, true);
	//绘制火花
	DrawFire();
	//输出到屏幕
	Print();
}
void EnemyTank_2()
{
	//坦克行走
	if (enemy2_tank.move == 0)
	{
		//行走
		Player_B.face = -1;
		Receive_Server();
		switch (Player_B.face)
		{			
		case DOWN:enemy2_tank.Change(DOWN);
			enemy2_tank.move = 64 / enemy2_tank.speed; break;
		case LEFT:enemy2_tank.Change(LEFT);
			enemy2_tank.move = 64 / enemy2_tank.speed; break;
		case UP:enemy2_tank.Change(UP);
			enemy2_tank.move = 64 / enemy2_tank.speed; break;
		case RIGHT:enemy2_tank.Change(RIGHT);
			enemy2_tank.move = 64 / enemy2_tank.speed; break;
		}
		if (enemy2_tank.move > 0)
		{
			bool hit = TankHitBlock(enemy2_tank);
			enemy2_tank.move = 64 / enemy2_tank.speed_real;
			if (hit == true)
			{
				enemy2_tank.move = 0;
				if (++enemy2_tank.move_step > 2)
					enemy2_tank.move_step = 0;
			}
			if (sound)PLAYB(MOTOR);
		}
	}
	if (enemy2_tank.move > 0)
	{
		--enemy2_tank.move;
		if (++enemy2_tank.move_step > 2)
			enemy2_tank.move_step = 0;
		enemy2_tank.Move();
		if (enemy2_tank.move == 0) {
			enemy2_tank.x = (enemy2_tank.x + 32) / 64 * 64;
			enemy2_tank.y = (enemy2_tank.y + 32) / 64 * 64;
		}
	}
	enemy2_tank.Draw(writeDC, player_tankDC, false);
}
#endif