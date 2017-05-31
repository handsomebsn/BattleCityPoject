#pragma once
#ifndef __MultiBattle__
#include"HEAD.h"
bool load_enemytank = true;
//��Ϸ��ʼ
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
//��ʼ��
void Init_2()
{
	MessageBoxA(hwnd, "��ս��ʼ", "", MB_OK);
	stage = 0;
	//BOSSģʽ
	boss_mode = false;
	//׼����صз�̹��
	load_enemytank = true;
	//���̹��
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
	//����Ϊ��̬��Դ���ͷ�
	//����ӵ����
	for (list<Bullet*>::iterator i = player_bullet.begin(); i != player_bullet.end(); ++i)
		delete *i;
	player_bullet.clear();
	//���������
	for (list<Fire*>::iterator i = fire.begin(); i != fire.end(); ++i)
		delete *i;
	fire.clear();
	//�����ڵ��������
	for (list<Bullet*>::iterator i = enemy2_bullet.begin(); i != enemy2_bullet.end(); ++i)
		delete *i;
	enemy2_bullet.clear();
	//�ؿ�
	//wintime = 0;
	//����
	base_time = 0;
	//��һָ�
	player_death = false;
	player_time = 0;
	//��������
	enemy_num = 0;
	enemy_total = 1;
	enemy_rest = enemy_total;
	enemy_time = 1;
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
		Key_2();
	//������Ϸ����
	DrawGame_2();
	//���ػ���
	if (base_time > 0) {
		if (--base_time == 0)
			game_state = GAMEOVER;
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
	//Item();
	//����ͻ��Ƶ���̹��
	EnemyTank_2();
	//����ͻ������̹��
	if (player_death == false)
		player_tank.Draw(writeDC, player_tankDC, false);
	//���Ʋݿ�
	DrawGrass();
	//����ͻ��Ƶ����ڵ�
	EnemyBullet_2();
	//����ͻ�������ڵ�
	PlayerBullet_2();
	//����ͻ��ƿ�������
	player_tank.Draw(writeDC, player_tankDC, true);
	//���ƻ�
	DrawFire();
	//�������Ļ
	Print();
}
//����ͻ��Ƶ���̹��
void EnemyTank_2()
{
	Player_B.face = -1;//����λ
	//Player_B.fire = false;//����λ
	//̹������
	if (enemy2_tank.move == 0)
	{
		if (clientORserver == 1)Receive_Server();
		if (clientORserver == 2)Receive_Client();
		//����
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
			else {				
				if (TankHitTank(enemy2_tank,player_tank ) == true)
				{
					enemy2_tank.move = 0;
					if (++enemy2_tank.move_step > 2)
						enemy2_tank.move_step = 0;
				}				
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
	//����	
	if (Player_B.fire) {
		//MessageBoxA(NULL, "TRUE", "", MB_OK); 
		Player_B.fire = false;
		//if (!enemy2_tank.gun_step && enemy2_tank.bullet_num < enemy2_tank.bullet_max)
		{
			if (sound)PLAYA(SHOOT);
			int x = enemy2_tank.x + (64 - 40) / 2;
			int y = enemy2_tank.y + (64 - 40) / 2;
			switch (enemy2_tank.gun_face)
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
			enemy2_bullet.push_back(new Bullet(enemy2_tank.bullet_id, 100, enemy2_tank.gun_face, x, y,
				enemy2_tank.bullet_speed, enemy2_tank.bullet_power));
			++enemy2_tank.bullet_num;
			enemy2_tank.Fire();
		}
	}
	enemy2_tank.Draw(writeDC, player_tankDC, false);
}

//����ͻ��Ƶ����ڵ����ڿ�����
void EnemyBullet_2()
{
	for (list<Bullet*>::iterator iter_bullet = enemy2_bullet.begin(); iter_bullet != enemy2_bullet.end();)
	{
		Bullet& bullet = **iter_bullet;
		bullet.Move();
		bool erase_bullet = false;
		//�ڵ��Ƿ�Խ��
		if (bullet.x + 76 - 1 < 0 || bullet.x - 36 > GAME_W - 1 || bullet.y + 76 - 1 < 0 || bullet.y - 36 > GAME_H - 1)
		{
			delete *iter_bullet;
			--enemy2_tank.bullet_num;
			iter_bullet = enemy2_bullet.erase(iter_bullet);
			erase_bullet = true;
		}
		//�Ƿ����ש��
		else if (BulletHitBlock(bullet) == true)
		{
			if (sound)PLAYA(HIT);
			fire.push_back(new Fire(bullet.x - (66 - 40) / 2, bullet.y - (66 - 40) / 2));
			delete *iter_bullet;
			--enemy2_tank.bullet_num;
			iter_bullet = enemy2_bullet.erase(iter_bullet);
			erase_bullet = true;
		}
		//���ڵ�û�б�ը�ͻ��Ƴ���
		if (erase_bullet == false)
			(*(iter_bullet++))->Draw(writeDC, bulletDC);
	}
}
//��������ڵ�
void PlayerBullet_2()
{
	for (list<Bullet*>::iterator iter_bullet = player_bullet.begin(); iter_bullet != player_bullet.end();)
	{
		Bullet& bullet = **iter_bullet;
		bullet.Move();
		bool erase_bullet = false;
		//�ڵ��Ƿ�Խ��
		if (bullet.x + 76 - 1 < 0 || bullet.x - 36 > GAME_W - 1 || bullet.y + 76 - 1 < 0 || bullet.y - 36 > GAME_H - 1)
		{
			delete *iter_bullet;
			--player_tank.bullet_num;
			iter_bullet = player_bullet.erase(iter_bullet);
			erase_bullet = true;
		}
		//�Ƿ����ש��
		else if (BulletHitBlock(bullet) == true)
		{
			if (sound)PLAYA(HIT);
			fire.push_back(new Fire(bullet.x - (66 - 40) / 2, bullet.y - (66 - 40) / 2));
			//�Ƿ���ез�����
			if (clientORserver == 1) {
				if (((bullet.x + 20) / 64 == 7) && ((bullet.y + 20) / 64 == 0))
					game_state = WIN;
			}
			else if (clientORserver == 2) {
				if (((bullet.x + 20) / 64 == 7) && ((bullet.y + 20) / 64 == 10))
					game_state = WIN;
			}
			delete *iter_bullet;
			--player_tank.bullet_num;
			iter_bullet = player_bullet.erase(iter_bullet);
			erase_bullet = true;
		}
		else {
			
		}
		//���ڵ�û�б�ը�ͻ��Ƴ���
		if (erase_bullet == false)
			(*(iter_bullet++))->Draw(writeDC, bulletDC);
	}
}
//�����ж�
void Key_2()
{
	//̹������
	if (player_tank.move == 0)
	{
		//����
		if (KEYDOWN(VK_DOWN)) {
			player_tank.Change(DOWN);
			player_tank.move = 64 / player_tank.speed;
			if (clientORserver == 2)Send_Client(MOVING,DOWN);//�����ź�
			if (clientORserver == 1)Send_Server(MOVING,DOWN);//�����ź�
		}
		else if (KEYDOWN(VK_LEFT)) {
			player_tank.Change(LEFT);
			player_tank.move = 64 / player_tank.speed;
			if (clientORserver == 2)Send_Client(MOVING,LEFT);//�����ź�
			if (clientORserver == 1)Send_Server(MOVING,LEFT);//�����ź�
		}
		else if (KEYDOWN(VK_UP)) {
			player_tank.Change(UP);
			player_tank.move = 64 / player_tank.speed;
			if (clientORserver == 2)Send_Client(MOVING,UP);//�����ź�
			if (clientORserver == 1)Send_Server(MOVING,UP);//�����ź�
		}
		else if (KEYDOWN(VK_RIGHT)) {
			player_tank.Change(RIGHT);
			player_tank.move = 64 / player_tank.speed;
			if (clientORserver == 2)Send_Client(MOVING,RIGHT);//�����ź�
			if (clientORserver == 1)Send_Server(MOVING,RIGHT);//�����ź�
		}
		if (player_tank.move > 0)
		{
			bool hit = TankHitBlock(player_tank);
			player_tank.move = 64 / player_tank.speed_real;
			if (hit == true)
			{
				player_tank.move = 0;
				if (++player_tank.move_step > 2)
					player_tank.move_step = 0;
			}
			else {
				if (TankHitTank(player_tank, enemy2_tank) == true)
				{
					player_tank.move = 0;
					if (++player_tank.move_step > 2)
						player_tank.move_step = 0;
				}
			}			
			if (sound)PLAYB(MOTOR);
		}
	}
	if (player_tank.move > 0)
	{
		--player_tank.move;
		if (++player_tank.move_step > 2)
			player_tank.move_step = 0;
		player_tank.Move();
		if (player_tank.move == 0) {
			player_tank.x = (player_tank.x + 32) / 64 * 64;
			player_tank.y = (player_tank.y + 32) / 64 * 64;
		}

	}
	//����
	if (KEYDOWN(VK_SPACE)) {
		if (!player_tank.gun_step && player_tank.bullet_num < player_tank.bullet_max)
		{
			if (clientORserver == 2)Send_Client(FIRING, TRUE);//�����ź�
			if (clientORserver == 1)Send_Server(FIRING, TRUE);//�����ź�

			if (sound)PLAYA(SHOOT);
			int x = player_tank.x + (64 - 40) / 2;
			int y = player_tank.y + (64 - 40) / 2;
			switch (player_tank.gun_face)
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
			player_bullet.push_back(new Bullet(player_tank.bullet_id, 100, player_tank.gun_face, x, y,
				player_tank.bullet_speed, player_tank.bullet_power));
			++player_tank.bullet_num;
			player_tank.Fire();
		}
	}
}
#endif