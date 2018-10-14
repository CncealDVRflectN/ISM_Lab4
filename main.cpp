#define GNUPLOT             "gnuplot"
#define GNUPLOT_WIN_WIDTH   1280
#define GNUPLOT_WIN_HEIGHT  720

#include <iostream>
#include <cmath>
#include "MultiplicativePRNG.h"

using namespace std;


const double INTEGRAL_VALUE = 1.378936078070656053;
const double DOUBLE_INTEGRAL_VALUE = 3.333333333333333333;


double calcIntegrand(double x)
{
    return exp(-x) * sqrt(1.0 + x);
}

double calcDoubleIntegrand(double x, double y)
{
    return x * x + y * y;
}


double calcIntegralMonteCarlo(double (*func)(double), double from, double to, int pointsNum)
{
    PRNG* prng = new MultiplicativePRNG(pow(2, 31), 262147, 262147);
    double result = 0.0;

    for (int i = 0; i < pointsNum; i++)
    {
        result += func(prng->next(from, to));
    }

    delete prng;

    return (to - from) * result / pointsNum;
}

double calcDoubleIntegralMonteCarlo(double (*func)(double, double), double firstFrom, double firstTo,
                                                                    double secondFrom, double secondTo,
                                                                    int pointsNum)
{
    PRNG* prng = new MultiplicativePRNG(pow(2, 31), 262147, 262147);
    double result = 0.0;

    for (int i = 0; i < pointsNum; i++)
    {
        result += func(prng->next(firstFrom, firstTo), prng->next(secondFrom, secondTo));
    }

    delete prng;

    return (firstTo - firstFrom) * (secondTo - secondFrom) * result / pointsNum;
}


int main()
{
    FILE* pipeIntegral;
    FILE* pipeDoubleIntegral;

#ifdef WIN32
    pipeIntegral = _popen(GNUPLOT, "w");
    pipeDoubleIntegral = _popen(GNUPLOT, "w");
#else
    pipeIntegral = popen(GNUPLOT, "w");
    pipeDoubleIntegral = popen(GNUPLOT, "w");
#endif

    const int pointsNumMin = 10;
    const int pointsNumStep = 10;
    const int pointsNumStepNum = 1000;
    const double integralFrom = 0.0;
    const double integralTo = 10.0;
    const double doubleIntFirstFrom = 0.0;
    const double doubleIntFirstTo = 1.0;
    const double doubleIntSecondFrom = 0.0;
    const double doubleIntSecondTo = 2.0;

    int pointsNum;

    if (pipeIntegral != nullptr && pipeDoubleIntegral != nullptr)
    {
        fprintf(pipeIntegral, "set term wxt size %d, %d\n", GNUPLOT_WIN_WIDTH, GNUPLOT_WIN_HEIGHT);
        fprintf(pipeIntegral, "set title \"Integral\"\n");
        fprintf(pipeIntegral, "set xlabel \"Number of points\"\n");
        fprintf(pipeIntegral, "set ylabel \"Error\"\n");
        fprintf(pipeIntegral, "plot '-' with line title \"Monte Carlo method error\"\n");

        pointsNum = pointsNumMin;
        for (int i = 0; i < pointsNumStepNum; i++)
        {
            fprintf(pipeIntegral, "%d %lf\n", pointsNum,
                    abs(INTEGRAL_VALUE - calcIntegralMonteCarlo(&calcIntegrand, integralFrom, integralTo, pointsNum)));
            pointsNum += pointsNumStep;
        }

        fprintf(pipeIntegral, "%s\n", "e");
        fflush(pipeIntegral);

        fprintf(pipeDoubleIntegral, "set term wxt size %d, %d\n", GNUPLOT_WIN_WIDTH, GNUPLOT_WIN_HEIGHT);
        fprintf(pipeDoubleIntegral, "set title \"Double integral\"\n");
        fprintf(pipeDoubleIntegral, "set xlabel \"Number of points\"\n");
        fprintf(pipeDoubleIntegral, "set ylabel \"Error\"\n");
        fprintf(pipeDoubleIntegral, "plot '-' with line title \"Monte Carlo method error\"\n");

        pointsNum = pointsNumMin;
        for (int i = 0; i < pointsNumStepNum; i++)
        {
            fprintf(pipeDoubleIntegral, "%d %lf\n", pointsNum,
                    abs(DOUBLE_INTEGRAL_VALUE - calcDoubleIntegralMonteCarlo(&calcDoubleIntegrand,
                            doubleIntFirstFrom, doubleIntFirstTo, doubleIntSecondFrom, doubleIntSecondTo, pointsNum)));
            pointsNum += pointsNumStep;
        }

        fprintf(pipeDoubleIntegral, "%s\n", "e");
        fflush(pipeDoubleIntegral);

        cin.clear();
        cin.get();

#ifdef WIN32
        _pclose(pipeIntegral);
        _pclose(pipeDoubleIntegral);
#else
        pclose(pipeIntegral);
        pclose(pipeDoubleIntegral);
#endif
    }
    else
    {
        printf("Could not open pipe");
    }

    return 0;
}