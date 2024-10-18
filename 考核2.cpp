#include <iostream>
#include <cmath>
#include <vector>
#include <corecrt_math_defines.h>

class BallisticsCalculator {
public:
    BallisticsCalculator(double v, double k, double m, double r, double p, double C, double g, double z, double x, double y)
        : v(v), k(k), m(m), r(r), p(p), C(C), g(g), z(z), x(x), y(y) {
        S = M_PI * r * r * 2; // 小弹丸与空气在一个方向上的接触面积
        k0 = C * p * S / 2;
        k1 = k0 / m;
        s = std::sqrt(x * x + y * y);
    }

    double calculateTime(double angle) const {
        return (std::exp(k1 * s) - 1) / (k1 * v * std::cos(angle));
    }

    double calculateZactual(double angle, double t) const {
        return v * t * std::sin(angle) - g * t * t / 2;
    }

    double solutionAngle() const {
        double angle, t, dz, za, ztemp;
        angle = std::atan2(z, s);
        ztemp = z;
        for (int i = 0; i < 20; ++i) {
            t = calculateTime(angle);
            za = calculateZactual(angle, t);
            dz = z - za;
            ztemp += dz;
            angle = std::atan2(ztemp, s);
            if (std::fabs(dz) < 1e-6) {
                return angle;
            }
        }
        return -1; // 无解
    }

private:
    double v, k, m, r, p, C, g, z, x, y;
    double S, k0, k1, s;
};

int main() {
    double v = 17;      // 小弹丸的初速度
    double k = 0.038;   // 25摄氏度下小弹丸的阻力系数，实际值会偏大
    double m = 3.2e-3;  // 小弹丸的质量，有0.1g以下的误差
    double r = 1.68e-2; // 小弹丸的直径，有0.2mm以下的误差
    double p = 1.169;   // 25摄氏度，标准大气压下的空气密度
    double C = 0.47;    // 球体在空气中的摩擦系数
    double g = 9.80;    // 重力加速度的默认值
    double z = 0.25;    // 目标高度
    double x = 3;       
    double y = 4;       

    BallisticsCalculator calculator(v, k, m, r, p, C, g, z, x, y);

    double angle = calculator.solutionAngle();
    if (angle == -1) {
        std::cout << "No solution!" << std::endl;
    } else {
        std::cout << "Final Angle: " << angle << std::endl;
    }

    return 0;
}