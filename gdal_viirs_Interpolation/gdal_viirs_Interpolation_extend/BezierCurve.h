#pragma once

#ifndef BEZIERCURVE_H
#define BEZIERCURVE_H
#include <vector>
#include <array>

namespace BezierCurveSpace {

    class BezierExecption		//异常类
    {
    private:
        const char* Message;
    public:
        BezierExecption(const char* msg) :Message(msg) {};
        const char* GetMessage() {
            return Message;
        }
    };


	class BezierPoint {
	public:
        BezierPoint() :x(0.0), y(0.0) {}
		BezierPoint(double _x, double _y):x(_x), y(_y) {}
        BezierPoint& operator = (const BezierPoint& other){
            this->x = other.x;
            this->y = other.y;
            return *this;
        }
        bool operator < (const BezierPoint& other) {

            if (this->x < other.x) {
                return true;
            }
            return false;
        }
	public:
		double x;
		double y;
	};

	class BezierCurve {
	public:
		BezierCurve(std::vector<BezierPoint> inputPoints) {
            curvePoints = new std::vector<BezierPoint>;
            createCurve(inputPoints);
        }
		~BezierCurve() {
            delete curvePoints;
        }

		// 实现单个点的插值  输入x  输出其对应的y值
        double SinglePointInterp(double x) {
            std::sort(curvePoints->begin(), curvePoints->end());

            for (const auto& point : *curvePoints)
            {
                if (point.x > x) {
                    return  point.y;
                }
            }

            throw BezierExecption("计算失败,在构造的曲线点集中未找到 插值x对应的y");
        }
        void createCurve(std::vector<BezierPoint>& originPoint) {
            //控制点收缩系数 ，经调试0.6较好，CvPoint是opencv的，可自行定义结构体(x,y)   
            float scale = 0.6;

            const int originCount = originPoint.size();
            std::vector<BezierPoint> midpoints(originCount);

            //生成中点        
            for (int i = 0;i < originCount; i++) {
                int nexti = (i + 1) % originCount;
                midpoints[i].x = (originPoint[i].x + originPoint[nexti].x) / 2.0;
                midpoints[i].y = (originPoint[i].y + originPoint[nexti].y) / 2.0;
            }

            //平移中点   
            std::vector<BezierPoint> extrapoints(2 * originCount);
            //CvPoint extrapoints[2 * originCount];
            for (int i = 0;i < originCount; i++) {
                int nexti = (i + 1) % originCount;
                int backi = (i + originCount - 1) % originCount;
                BezierPoint midinmid;
                midinmid.x = (midpoints[i].x + midpoints[backi].x) / 2.0;
                midinmid.y = (midpoints[i].y + midpoints[backi].y) / 2.0;
                int offsetx = originPoint[i].x - midinmid.x;
                int offsety = originPoint[i].y - midinmid.y;
                int extraindex = 2 * i;
                extrapoints[extraindex].x = midpoints[backi].x + offsetx;
                extrapoints[extraindex].y = midpoints[backi].y + offsety;
                //朝 originPoint[i]方向收缩    
                int addx = (extrapoints[extraindex].x - originPoint[i].x) * scale;
                int addy = (extrapoints[extraindex].y - originPoint[i].y) * scale;
                extrapoints[extraindex].x = originPoint[i].x + addx;
                extrapoints[extraindex].y = originPoint[i].y + addy;

                int extranexti = (extraindex + 1) % (2 * originCount);
                extrapoints[extranexti].x = midpoints[i].x + offsetx;
                extrapoints[extranexti].y = midpoints[i].y + offsety;
                //朝 originPoint[i]方向收缩    
                addx = (extrapoints[extranexti].x - originPoint[i].x) * scale;
                addy = (extrapoints[extranexti].y - originPoint[i].y) * scale;
                extrapoints[extranexti].x = originPoint[i].x + addx;
                extrapoints[extranexti].y = originPoint[i].y + addy;

            }
            std::array<BezierPoint, 4> controlPoint{};
            size_t  s;
            //CvPoint controlPoint[4];
            //生成4控制点，产生贝塞尔曲线   
            for (int i = 0;i < originCount; i++) {
                controlPoint[0].x = originPoint[i].x;
                controlPoint[0].y = originPoint[i].y;
                int extraindex = 2 * i;
                controlPoint[1].x = extrapoints[extraindex + 1].x;
                controlPoint[1].y = extrapoints[extraindex + 1].y;
                int extranexti = (extraindex + 2) % (2 * originCount);
                controlPoint[2].y = extrapoints[extranexti].x;
                controlPoint[2].y = extrapoints[extranexti].y;
                int nexti = (i + 1) % originCount;
                controlPoint[3].x = originPoint[nexti].x;
                controlPoint[3].y = originPoint[nexti].y;
                float u = 1;
                while (u >= 0) {
                    double px = bezier3funcX(u, controlPoint);
                    double py = bezier3funcY(u, controlPoint);
                    //u的步长决定曲线的疏密   
                    //u -= 0.005;
                    u -= 0.1;
                    BezierPoint tempP = BezierPoint(px, py);
                    //存入曲线点    
                    curvePoints->push_back( tempP);
                }
            }
        }
		//三次贝塞尔曲线   
		float bezier3funcX(float uu, std::array<BezierPoint, 4>& controlP) {
			float part0 = controlP[0].x * uu * uu * uu;
			float part1 = 3 * controlP[1].x * uu * uu * (1 - double(uu));
			float part2 = 3 * controlP[2].x * uu * (1 - uu) * (1 - uu);
			float part3 = controlP[3].x * (1 - uu) * (1 - uu) * (1 - uu);
			return part0 + part1 + part2 + part3;
		}
		float bezier3funcY(float uu, std::array<BezierPoint, 4>& controlP) {
			float part0 = controlP[0].y * uu * uu * uu;
			float part1 = 3 * controlP[1].y * uu * uu * (1 - uu);
			float part2 = 3 * controlP[2].y * uu * (1 - uu) * (1 - uu);
			float part3 = controlP[3].y * (1 - uu) * (1 - uu) * (1 - uu);
			return part0 + part1 + part2 + part3;
		}
	private:
		std::vector<BezierPoint>* curvePoints;
	};


}


#endif // BEZIERCURVE_H
