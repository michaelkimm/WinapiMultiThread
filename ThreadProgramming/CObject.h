#pragma once
#ifndef CSHAPE_H_
#define CSHAPE_H_

#include <windows.h>
#include <iostream>
#include <vector>
#include <math.h>

using namespace std;



class CObject
{
protected:
	float px, py;
	float vx, vy;
	float ax, ay;
	SIZE collisionSize;
	float mass;
	int type;

public:
	enum type { no_type, circle, star, rectangle };

	CObject();
	CObject(float _px, float _py, float _vx, float _vy, float _ax, float _ay,
		SIZE _collisionSize, float _mass, int _type);
	virtual	~CObject();

	float getPx() const;
	float getPy() const;
	float getVx() const;
	float getVy() const;
	float getAx() const;
	float getAy() const;
	float getMass()	const;
	int getType() const;
	SIZE getCollisionSize();
	void setPx(float);
	void setPy(float);
	void setVx(float);
	void setVy(float);
	void setAx(float);
	void setAy(float);
	
	void setCollisionSize(SIZE);
	void setMass(float _mass);

	virtual void		Update(int milis) = 0;						// milis sec마다 상태 갱신
	virtual void		Collison(CObject * _obj) = 0;				// 충돌 판정 후 위치 조정
	virtual void		CollisionWall(const RECT & border) = 0;		// 벽 충돌 반정 후 위치 조정
	void				bounce(CObject * _obj, float _fDistance);

	virtual void		Draw(HDC hdc) const = 0;
};


class CCircle : public CObject
{
private:
	float radius;

public:
	CCircle() : CObject(), radius(0.f) {}
	CCircle(float _px, float _py, float _vx, float _vy, float _ax, float _ay, SIZE _collisionSize, float _mass, int _type, float _radius)
		: CObject(_px, _py, _vx, _vy, _ax, _ay, _collisionSize, _mass, _type), radius(_radius) { }
	~CCircle() {}

	float	getRadius() const { return radius; }
	float	setRadius(float _radius) { radius = _radius; }

	virtual void	Update(int milis);
	virtual void	Collison(CObject * _obj);
	virtual void	CollisionWall(const RECT & border);
	virtual void	Draw(HDC hdc) const;
};

class SStar : public CCircle
{
private:
	int vertexN;

public:
	SStar() : CCircle(), vertexN(5) {}
	SStar(float _px, float _py, float _vx, float _vy, float _ax, float _ay, SIZE _collisionSize, float _mass, int _type, float _radius, int _vertexN)
		: CCircle(_px, _py, _vx, _vy, _ax, _ay, _collisionSize, _mass, _type, _radius), vertexN(_vertexN) {}
	~SStar() {}

	int		getVertexN() const { return vertexN; }
	void	setVertexN(int _vertexN) { vertexN = _vertexN; }

	virtual void	Draw(HDC hdc) const;
};


class RRectangle : public CObject
{
private:
	float width;
	float height;
	float rotated_rad_;			
	float rotating_speed_rad_;		// rad/s

public:
	RRectangle() : CObject(), width(0.f), height(0.f), rotated_rad_(0.f), rotating_speed_rad_(0.f) {}
	RRectangle(float _px, float _py, float _vx, float _vy, float _ax, float _ay, SIZE _collisionSize, float _mass, int _type, float _width, float _height, float _rotated_rad, float _rotating_speed_rad)
		: CObject(_px, _py, _vx, _vy, _ax, _ay, _collisionSize, _mass, _type), width(_width), height(_height), rotated_rad_(_rotated_rad), rotating_speed_rad_(_rotating_speed_rad) { }
	~RRectangle() {}

	float getWidth() const				{ return width; }
	float getHeight() const				{ return height; }
	float getRotatedRad() const			{ return rotated_rad_; }
	float getRotatingSpeedRad() const	{ return rotating_speed_rad_; }

	void setWidth(float _width)							{ width = _width; }
	void setHeight(float _height)						{ height = _height; }
	void setRotatedRad(float _rotated_rad)				{ rotated_rad_ = _rotated_rad; }
	void setRotatingSpeedRad(float _rotated_speed_rad)	{ rotating_speed_rad_ = _rotated_speed_rad; }

	void			Update(int milis);						// milis sec마다 상태 갱신
	virtual void	Collison(CObject * _obj);
	virtual void	CollisionWall(const RECT & border);

	virtual void	Draw(HDC hdc) const;
};

#endif
