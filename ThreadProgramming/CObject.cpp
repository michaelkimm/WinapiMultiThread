
#include "CObject.h"
#include "figureDraw.h"
#include <iostream>

#ifndef PI
#define PI 3.141592
#endif

// CObject 클래스 정의
//----------------------------------------------------------------------------------------------------//
CObject::CObject()
	: px(0.f), py(0.f), vx(0.f), vy(0.f), ax(0.f), ay(0.f), collisionSize(SIZE{ 0, 0 }), mass(1.f), type(no_type) {}


CObject::CObject(float _px, float _py, float _vx, float _vy, float _ax, float _ay, SIZE _collisionSize, float _mass, int _type)
	: px(_px), py(_py), vx(_vx), vy(_vy), ax(_ax), ay(_ay), collisionSize(_collisionSize), mass(_mass), type(no_type) {}

CObject::~CObject() {}
float CObject::getPx() const { return px; }
float CObject::getPy() const { return py; }
float CObject::getVx() const { return vx; }
float CObject::getVy() const { return vy; }
float CObject::getAx() const { return ax; }
float CObject::getAy() const { return ax; }
SIZE CObject::getCollisionSize() { return collisionSize; }
float CObject::getMass() const { return mass; }
int CObject::getType() const { return type; }
void CObject::setPx(float _px) { px = _px; }
void CObject::setPy(float _py) { py = _py; }
void CObject::setVx(float _vx) { vx = _vx; }
void CObject::setVy(float _vy) { vy = _vy; }
void CObject::setAx(float _ax) { ax = _ax; }
void CObject::setAy(float _ay) { ay = _ay; }
void CObject::setCollisionSize(SIZE _collisionSize) { collisionSize = _collisionSize; }
void CObject::setMass(float _mass) { mass = _mass; }

void CObject::bounce(CObject * _obj, float _fDistance)
{
	// 노말 벡터 계산
	float nx = (_obj->getPx() - getPx()) / _fDistance;
	float ny = (_obj->getPy() - getPy()) / _fDistance;

	// 탄젠트 벡터 계산
	float tx = -ny;
	float ty = nx;

	// 탄젠트 벡터와 각 원 속도 내적 계산
	float dpTan1 = getVx() * tx + getVy() * ty;
	float dpTan2 = _obj->getVx() * tx + _obj->getVy() * ty;

	// 노멀방향 속도 내적 계산
	float dpNorm1 = getVx() * nx + getVy() * ny;
	float dpNorm2 = _obj->getVx() * nx + _obj->getVy() * ny;

	// float m1 = (dpNorm1 * (getMass() - _obj->getMass()) + 2.0f * _obj->getMass() * dpNorm2) / (getMass() + _obj->getMass());
	// float m2 = (dpNorm2 * (_obj->getMass() - getMass()) + 2.0f * getMass() * dpNorm1) / (getMass() + _obj->getMass());

	float m1 = dpNorm2;
	float m2 = dpNorm1;

	// 위에서 내적한 값 만큼 탄젠트 벡터 방향 속도를 가진다. (속도 업데이트)
	setVx(tx * dpTan1 + nx * m1);
	setVy(ty * dpTan1 + ny * m1);
	_obj->setVx(tx * dpTan2 + nx * m2);
	_obj->setVy(ty * dpTan2 + ny * m2);
}


// CCircle 클래스 정의
//----------------------------------------------------------------------------------------------------//
void CCircle::Update(int milis)
{
	float sec = milis * 0.001;
	// WM_TIMER에 한번씩 움직이는 업데이트
	px += vx * sec + 0.5 * ax * sec * sec;
	py += vy * sec + 0.5 * ay * sec * sec;
	vx += ax * sec;
	vy += ay * sec;
	ax = 0;
	ay = 0;
}

void CCircle::Collison(CObject * _obj)	// 중점, 질량, 반지름 사용
{
	CCircle * pt_cir = dynamic_cast<CCircle *>(_obj);

	if (pt_cir)
	{
		float fDistance = sqrtf((px - pt_cir->getPx())*(px - pt_cir->getPx()) + (py - pt_cir->getPy())*(py - pt_cir->getPy()));
		float foverlap = 0.5 * (getRadius() + pt_cir->getRadius() - fDistance);

		if (doCirclesOverlap(getPx(), getPy(), getRadius(), pt_cir->getPx(), pt_cir->getPy(), pt_cir->getRadius()))
		{
			// 노말 벡터 계산
			float nx = (pt_cir->getPx() - getPx()) / fDistance;
			float ny = (pt_cir->getPy() - getPy()) / fDistance;

			// 겹친 부분 조정
			px -= foverlap * (pt_cir->getPx() - getPx()) / fDistance;
			py -= foverlap * (pt_cir->getPy() - getPy()) / fDistance;
			pt_cir->px += foverlap * (pt_cir->getPx() - getPx()) / fDistance;
			pt_cir->py += foverlap * (pt_cir->getPy() - getPy()) / fDistance;

			// 탄성 충돌로 인한 속도 업데이트
			bounce(pt_cir, fDistance);
		}
	}

	RRectangle * pt_rec = dynamic_cast<RRectangle *>(_obj);

	if (pt_rec)
	{

	}
	
	
}

void CCircle::CollisionWall(const RECT & border)
{

	// 벽 부딛히면 통과
	if (getPx() > border.right)
		setPx(0);
	else if (getPx() < 0)
		setPx(border.right);
	else if (getPy() > border.bottom)
		setPy(0);
	else if (getPy() < 0)
		setPy(border.bottom);
}

void CCircle::Draw(HDC hdc) const
{
	DrawEllipse(hdc, POINT{ (LONG)px, (LONG)py }, (LONG)radius);
}


// SStar 클래스 정의
//----------------------------------------------------------------------------------------------------//
void SStar::Draw(HDC hdc) const
{
	DrawStarN(hdc, POINT{ (LONG)getPx(), (LONG)getPy() }, (LONG)getRadius(), getVertexN());
}



// RRectangle 클래스 정의
//----------------------------------------------------------------------------------------------------//
void RRectangle::Update(int milis)
{
	float sec = milis * 0.001f;
	// WM_TIMER에 한번씩 움직이는 업데이트
	setPx(getPx() + getVx() * sec + 0.5 * getAx() * sec * sec);
	setPy(getPy() + getVy() * sec + 0.5 * getAy() * sec * sec);
	setVx(getVx() + getAx() * sec);
	setVy(getVy() + getAy() * sec);
	setAx(0.f);
	setAy(0.f);
	setRotatedRad(getRotatedRad() + getRotatingSpeedRad() * sec);
}

void RRectangle::Collison(CObject * _obj)
{
	RRectangle * pt_rec = dynamic_cast<RRectangle *>(_obj);

	if (pt_rec)
	{
		// 폴리곤 점들을 저장하는 poly 배열 생성 후 값 저장
		POINT object1Pt[4] = { NULL, };
		POINT object2Pt[4] = { NULL, };
		getRectPoints(POINT{ (LONG)getPx(), (LONG)getPy()}, (LONG)getWidth(), (LONG)getHeight(), getRotatedRad(), object1Pt);
		getRectPoints(POINT{ (LONG)(pt_rec->getPx()), (LONG)(pt_rec->getPy()) }, (LONG)(pt_rec->getWidth()), (LONG)(pt_rec->getHeight()), pt_rec->getRotatedRad(), object2Pt);

		// 대각선 거리 안에 사각형이 있을 때만 판단
		if (doCirclesOverlap(getPx(), getPy(), getL2N(getWidth() / 2, getHeight() / 2),
				pt_rec->getPx(), pt_rec->getPy(), getL2N(pt_rec->getWidth() / 2, pt_rec->getHeight() / 2)))
		{
			// <sat 알고리즘으로 겹침 판단>
			// 반환 받을 값: shortest_foverlap, shortest_normal_vector
			float shortest_foverlap = 3.4E+37f;
			std::pair<float, float> shortest_normal_vector{ 0.f, 0.f };	// first = x, second = y
			// 알고리즘 시작
			if (satAlgorithm(object1Pt, object2Pt, shortest_foverlap, shortest_normal_vector))
			{
				shortest_foverlap /= 2;		// 각자 튕겨나갈 거리
				// 겹친 부분 조정
				setPx(getPx() - shortest_normal_vector.first * shortest_foverlap);
				setPy(getPy() - shortest_normal_vector.second * shortest_foverlap);
				pt_rec->setPx(pt_rec->getPx() + shortest_normal_vector.first * shortest_foverlap);
				pt_rec->setPy(pt_rec->getPy() + shortest_normal_vector.second * shortest_foverlap);
				// 탄성 충돌
				bounce(pt_rec, getfDistance(POINT{ (LONG)getPx(), (LONG)getPy() }, POINT{ (LONG)_obj->getPx(), (LONG)_obj->getPy() }));
			}
		}
	}

	CCircle * pt_cir = dynamic_cast<CCircle *>(_obj);

	if (pt_cir)
	{

	}
}
void RRectangle::CollisionWall(const RECT & border)
{
	// 벽 부딛히면 통과
	if (getPx() > border.right)
		setPx(0);
	else if (getPx() < 0)
		setPx(border.right);
	else if (getPy() > border.bottom)
		setPy(0);
	else if (getPy() < 0)
		setPy(border.bottom);
}

void RRectangle::Draw(HDC hdc) const
{
	DrawRectangle(hdc, POINT{ (LONG)getPx(), (LONG)getPy() }, getWidth(), getHeight(), getRotatedRad());
}
