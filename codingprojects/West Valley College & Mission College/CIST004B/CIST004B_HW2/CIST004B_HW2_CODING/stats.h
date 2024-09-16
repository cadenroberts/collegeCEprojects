#pragma once
#ifndef STATS_H     // Prevent duplicate definition
#define STATS_H

#include <iostream>
namespace main_savitch_2C
{
    class statistician
    {
    public:
        statistician();
        void next(double r);
        void reset();
        int length() const;
        double sum() const;
        double mean() const;
        double minimum() const;
        double maximum() const;
        friend statistician operator +
            (const statistician& s1, const statistician& s2);
        friend statistician operator *
            (double scale, const statistician& s);
    private:
        int count = 0;       // How many numbers in the sequence
        double total = 0;    // The sum of all the numbers in the sequence
        double tinyest = 0;  // The smallest number in the sequence
        double largest = 0;  // The largest number in the sequence
    };
    bool operator ==(const statistician& s1, const statistician& s2);
}
#endif
