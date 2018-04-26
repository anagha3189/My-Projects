#ifndef SHAPE_H
#define SHAPE_H

class Shape
{
	public:
	virtual ~Shape() {}
	virtual const char* name() const = 0;
	virtual int sides() const = 0;
};

class Square : public Shape
{
	public:
	Square(int x, int y , int width , int height)
	: x_(x), y_(y), width_(width), height_(height);
	virtual ~Square() {}

	const char* name() const {return "Square"};
	int sides() const {return 4 }:
	
	int x() const {return x_};
	int y() const {return y_};
	int weight() const {return weight_};
	int height() const {return height_};

	private:
	int x_;
	int y_;
	int weight_;
	int height_;
};

#endif
