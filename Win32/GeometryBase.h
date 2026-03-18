#pragma once
#include <vector>

struct Point {
	double x, y;
};

class GeometryBase {
public:
	GeometryBase(Point begin, Point end) {
		beginPos = begin;
		endPos = end;
	};
	~GeometryBase() {
		delete[] points.data();
	};
	virtual std::vector<Point> GetPoints() = 0;
	//数据
	std::vector<Point> points;
	Point beginPos;
	Point endPos;
private:	
};