#pragma once
#ifndef __MultiBattle__
#include"HEAD.h"
//游戏开始
void Start_2()
{
	SetTimer(hwnd, GAME_ID, GAME_TIME, NULL);
	player_tank = BaseTank(20 / 10 - 1, 20, 0, UP, 5 * 64, 10 * 64 + 64, UP, false, 16, 4, 8, 2, 1, 10, 8);
	game_state = GAME;
	memset(&Player_B, 0, sizeof(Player_B));//清空结构体
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
	}else if(clientORserver==2)
	{
		player_tank = BaseTank(
			player_tank.id,	player_tank.life, player_tank.armor,
			DOWN, 9 * 64, -1 * 64 + 0,	DOWN,
			player_tank.gun_lock,
			64 / player_tank.speed,	player_tank.speed,
			player_tank.fire_speed,
			player_tank.bullet_id,	player_tank.bullet_max,
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
	//MessageBoxA(NULL, "调试", "", MB_OK);
}
//游戏过程
void Game_2()
{
	if(clientORserver==1)Receive_Server();
	if(clientORserver==2)Receive_Client();
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
	//少于屏幕最大敌人数并且剩余敌人数大于零添加敌人
	if (enemy_num < enemy_maxscr && enemy_time == 0 && enemy_rest>0)
	{
		enemy_time = enemy_timemax;
	}
	if (enemy_time > 0)
	{
		--enemy_time;
		if (enemy_time == 0)
		{
			++enemy_num;
			--enemy_rest;
			if(clientORserver==1)
			enemy_tank.push_back(new BaseTank(GOLD, 20, 0, DOWN, 9 * 64, -1 * 64 + 0, DOWN, false, 16, 4, 8, 2, 1, 10, 8));
			else if(clientORserver==2)
			enemy_tank.push_back(new BaseTank(GOLD, 20, 0, UP, 5 * 64, 10 * 64 + 64,UP, false, 16, 4, 8, 2, 1, 10, 8));
			//添加子弹容器
			enemy_bullet.push_back(new list<Bullet*>);
			//刷新剩余量绘图
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
	list<BaseTank*>::iterator iter_tank;
	list<list<Bullet*>*>::iterator iter_bullet;
	//按顺序处理所有敌方坦克
	for (iter_tank = enemy_tank.begin(),
		iter_bullet = enemy_bullet.begin();
		iter_tank != enemy_tank.end();
		++iter_tank,
		++iter_bullet)
	{
		BaseTank& t = **iter_tank;
		//开火
		if (t.gun_step == 0 && t.bullet_num < t.bullet_max && rand() % 20 == 0)
		{
			//PLAYA(SHOOT)
			int x = t.x + (64 - 40) / 2;
			int y = t.y + (64 - 40) / 2;
			switch (t.gun_face)
			{
			case DOWN:
				y += 47;
				break;
			case LEFT:
				x -= 47;
				break;
			case UP:
				y -= 47;
				break;
			case RIGHT:
				x += 47;
			}
			(**iter_bullet).push_back(new Bullet(t.bullet_id, 100, t.gun_face, x, y,
				t.bullet_speed, t.bullet_power));
			++t.bullet_num;
			t.Fire();
		}
		int oldface = t.face;
		//当步数减为零并且炮口闪光消失时随机生成移动方向和步数
		if (t.delay > 0)
		{
			if (++t.move_step > 2)
				t.move_step = 0;
			--t.delay;
		}
		if (t.move == 0 && t.delay == 0)
		{
			t.Change(Player_B.face);//转换方向
			t.move = 64 / t.speed;
			//撞到砖块
			if (TankHitBlock(t) == true)
			{
				t.move = 0;
				t.delay = 5;
			}
			//撞到玩家坦克
			else if (player_death == false && TankHitTank(t, player_tank) == true)
			{
				//t.Change(oldface);
				t.move = 0;
				t.delay = 5;
			}
			//如果撞到敌方任意非己坦克……
			else
			{
				for (list<BaseTank*>::iterator i = enemy_tank.begin(); i != enemy_tank.end(); ++i)
					//如果不是自己本身并且撞到对方
					if (i != iter_tank && TankHitTank(t, **i) == true)
					{
						//t.Change(oldface);
						t.move = 0;
						break;
					}
			}
			if (t.move > 0)
				t.move = 64 / t.speed_real;
		}
		if (t.move > 0)
		{
			--t.move;
			if (++t.move_step > 2)
				t.move_step = 0;
			t.Move();
		}
		t.Draw(writeDC, player_tankDC, false);
	}
}
#endif