/*
 * Min.h
 *
 *  Created on: 28.09.2011
 *      Author: s3-syn
 */

#ifndef MIN_H_
#define MIN_H_

#include "../core/Boost.h"
#include "UnivariateFunction.h"

class Bracket;

class Min {
public:
    Min();
    virtual ~Min();

    static shared_ptr<Bracket> brack(const shared_ptr<UnivariateFunction> function, double a, double b, shared_ptr<Bracket> bracket) {
        return bracket;
    }

    static bool brent(shared_ptr<UnivariateFunction> function, shared_ptr<Bracket> bracket, double relativeAccuracyGoal) {
        return false;
    }

    static const double GOLDEN;
};

class Bracket {
public:

    Bracket() {};
    Bracket(double lowerX, double upperX, shared_ptr<UnivariateFunction> function) {
        if(lowerX > upperX) {
            this->lowerX = upperX;
            this->upperX = lowerX;
        } else {
            this->lowerX = lowerX;
            this->upperX = upperX;
        }

        lowerF = function->value(this->lowerX);
        upperF = function->value(this->upperX);

        minimumX = this->lowerX + Min::GOLDEN * (this->upperX - this->lowerX);
        minimumF = function->value(this->minimumX);
    };

    double lowerX;
    double upperX;
    double minimumX;

    double lowerF;
    double upperF;
    double minimumF;

};

#endif /* MIN_H_ */
