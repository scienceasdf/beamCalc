#ifndef beam_H
#define beam_H

#include<map>
#include<set>
#include<numeric>

class beam{
public:
    double rRestraint=0;
    std::map<double,double> loads;
    std::map<double,double> meLoads;
    double leng,Ela,intia;
    std::set<double> jizhongPos;
    beam(double length, double e, double Iz):leng(length), Ela(e), intia(Iz) {};

    virtual double wanju(const double& pos)=0;
    virtual double neili(const double&)=0;
    virtual double bending(const double&)=0;
    virtual void calc();

    double bend[101]={0};
    double Fsy[101]={0};
    double innerMz[101]={0};

    void loadjizhongli(double& pos, double& force);

    void loadMoment(double& pos, double& me);

    void loadlin(double& pos1,double& pos2,double& force1,double& force2);

};


class simpSupBeam:public beam{
public:
    simpSupBeam(double length, double e, double Iz):beam(length,e,Iz) {};
    //simpSupBeam():beam() {};
    //void calc();
    double bending(const double& pos);
    double wanju(const double& pos);
    double neili(const double&);

};

//cantilever beam

class caBeam:public beam{
public:
    caBeam(double length, double e, double Iz):beam(length,e,Iz) {};
    //caBeam():beam() {};
    double bending(const double& pos);
    double wanju(const double& pos);
    double neili(const double&);


};


class overHangingBeam : public beam{
public:


    overHangingBeam(double length, double e, double Iz,double rRes):beam(length,e,Iz){rRestraint=rRes; }
    overHangingBeam(double length, double e, double Iz):beam(length,e,Iz){rRestraint=length/2.0; }

    double wanju(const double& pos);
    double neili(const double&);

    //void calc();
    double bending(const double&); //use the same name as the base class's virtual function
    //but it is used for getting the theta of the start point
    //double tryBending(double&& theta);

};

#endif // beam_H
