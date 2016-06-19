#include"beam.h"
#include<fstream>

void beam::loadjizhongli(double& pos, double& force)
{
    if(force!=0){
        jizhongPos.insert(pos);
        loads[pos]+=force;
        //for(int i=0;i<51;i++){

        //}
    }
}

void beam::loadMoment(double& pos, double& me)
{
    if(me!=0){
        jizhongPos.insert(pos);
        meLoads[pos]+=me;
    }
}

void beam::loadlin(double& pos1,double& pos2,double& force1,double& force2)
{
    for(int i=0;i<201;i++)
    {
        loads[pos1+i*(pos2-pos1)/200.0]+=(force1+i*(force2-force1)/200.0)*(pos2-pos1)/200;
    }
}

void beam::calc()
{
    double k=leng/100.0;
    for(int i=0;i<101;i++){
        bend[i]=bending(i*k);
        Fsy[i]=neili(i*k);
        innerMz[i]=wanju(i*k);
    }
}

//----------------------------------------------------------------------------------------------------

double simpSupBeam::bending(const double& pos)
{
    #define left iter->first
    #define force iter->second
    #define right (leng-iter->first)

    double v=0;
    std::map<double,double>::iterator p=loads.end();
    for(std::map<double,double>::iterator iter=loads.begin() ;iter!=p;++iter)
    {
           v+=(pos<left)?-force*right*pos*(leng*leng-pos*pos-right*right)/6.0:
               -force*right*(leng/right*(pos-left)*(pos-left)*(pos-left)+(leng*leng-right*right)*pos-pos*pos*pos)/6.0;
    }
    #undef force

    p=meLoads.end();
    #define me (iter->second)

    for(std::map<double,double>::iterator iter=meLoads.begin();iter!=p;++iter)
    {
        v+=(pos<left)?me*pos*(leng*leng-3.0*right*right-pos*pos)/6.0:
            me*(-pos*pos*pos+3*leng*(pos-left)*(pos-left)+(leng*leng-3.0*right*right)*pos)/6.0;
    }

    return v/Ela/intia/leng;

    #undef left
    #undef me
    #undef right
}

double simpSupBeam::wanju(const double& pos)
{
    double f=0;
    std::map<double,double>::iterator p=loads.end();
    for(std::map<double,double>::iterator iter=loads.begin() ;iter!=p;++iter)
    {
        f+=(pos<iter->first)?pos*(leng-iter->first)*iter->second/leng:
            (leng-pos)*(iter->first)*iter->second/leng;
    }
    p=meLoads.end();
    for(std::map<double,double>::iterator iter=meLoads.begin() ;iter!=p;++iter)
    {
        f+=(pos<iter->first)?-pos*iter->second/leng:-pos*iter->second/leng+iter->second;
    }

    return f;
}

double simpSupBeam::neili(const double& pos)
{
    double f=0;
    std::map<double,double>::iterator p=loads.end();
    for(std::map<double,double>::iterator iter=loads.begin() ;iter!=p;++iter)
    {
        f+=(pos<iter->first)?-(leng-iter->first)*iter->second/leng:
            (iter->first)*iter->second/leng;
    }
    p=meLoads.end();
    for(std::map<double,double>::iterator iter=meLoads.begin() ;iter!=p;++iter){
        f+=iter->second/leng;
    }
    return f;
}

//----------------------------------------------------------------------------------------------------

double caBeam::bending(const double& pos)
{
    #define left iter->first
    #define force iter->second
    #define right (leng-iter->first)

    double v=0;
    std::map<double,double>::iterator p=loads.end();
    for(std::map<double,double>::iterator iter=loads.begin() ;iter!=p;++iter)
    {
           v+=(pos<left)?-force*pos*pos*(3.0*left-pos)/6.0:
               -force*left*left*(3.0*pos-left)/6.0;
    }
    #undef force

    p=meLoads.end();
    #define me (iter->second)

    for(std::map<double,double>::iterator iter=meLoads.begin();iter!=p;++iter)
    {
        v+=(pos<left)?-me*pos*pos/2.0:
            -me*left*(pos-left/2.0);
    }


    return v/Ela/intia;

    #undef left
    #undef me
    #undef right
}

double caBeam::wanju(const double& pos)
{
    double f=0;
    std::map<double,double>::iterator p=loads.end();
    for(std::map<double,double>::iterator iter=loads.begin() ;iter!=p;++iter)
    {
        f+=(pos<iter->first)?-iter->first*iter->second+pos*iter->second:0;
    }
    p=meLoads.end();
    for(std::map<double,double>::iterator iter=meLoads.begin() ;iter!=p;++iter)
    {
        f+=(pos<iter->first)?-iter->second:0;
    }
    return f;
}

double caBeam::neili(const double& pos){
    double f=0;
    std::map<double,double>::iterator p=loads.end();
    for(std::map<double,double>::iterator iter=loads.begin();iter!=p;++iter){
        f+=(pos<iter->first)?-iter->second:0;
    }
    return f;
}

//----------------------------------------------------------------------------------------

double overHangingBeam::wanju(const double& pos)
{
    double f=0;
    std::map<double,double>::iterator p=loads.end();
    for(std::map<double,double>::iterator iter=loads.begin() ;iter!=p;++iter)
    {
        if((iter->first)<rRestraint){
            f+=(pos<rRestraint)?((pos<iter->first)?pos*(rRestraint-iter->first)*iter->second/rRestraint:
                (rRestraint-pos)*(iter->first)*iter->second/rRestraint) : 0;}
        else{
            f-=(pos<iter->first)?((pos<rRestraint)?pos*(iter->first-rRestraint)*iter->second/rRestraint:
                                                   iter->second*(leng-pos)-iter->second*(leng-iter->first)) : 0;
        }


    }
    p=meLoads.end();
    for(std::map<double,double>::iterator iter=meLoads.begin() ;iter!=p;++iter)
    {
        f+=(pos<iter->first)?-pos*iter->second/leng:-pos*iter->second/leng+iter->second;
    }

    return f;
}

/*double overHangingBeam::bending(const double& z0)
{
    const double x0=0.0;
    const double y0=0.0;

    const int nn=100;
    double h=rRestraint/nn;
    double x1=x0;
    double y1=y0;
    double z1=z0;
    double m1,m2,m3,m4,k1,k2,k3,k4;    
    for(int n=1;n<(nn+1);n++)
    {
        k1=h*z1;
        m1=h*wanju(x1);
        k2=h*(z1+m1/2.0);
        m2=h*wanju(x1+h/2.0);
        k3=h*(z1+m2/2.0);
        m3=h*wanju(x1+h/2.0);
        k4=h*(z1+m3);
        m4=h*wanju(x1+h);
        y1+=(k1+2.0*k2+2.0*k3+k4)/6.0;
        z1+=(m1+2.0*m2+2.0*m3+m4)/6.0;
        x1+=h;
    }
    return (y1);
}*/



/*void overHangingBeam::calc()
{
    const double eps=.0000001;
    double x0=.1,x1=.15;
    double x,y=10;
    //solving the theta of the start point
    do
    {
        x=x1;
        y=bending(x1);
        x1=x1-y/(y-bending(x0))*(x1-x0);
        x0=x;
    } while(fabs(y)>eps);

    //x1 is the theta of the start point
    double z1=x1;
    bend[0]=0;
    double h=leng/100;
    x1=0;
    double y1=0;
    double k=leng/100.0;
    double m1,m2,m3,m4,k1,k2,k3,k4;
    //std::ofstream f1("d:\\me.txt");
    for(int n=1;n<(101);++n)
    {
        k1=h*z1;
        m1=h*wanju(x1);
        k2=h*(z1+m1/2.0);
        m2=h*wanju(x1+h/2.0);
        k3=h*(z1+m2/2.0);
        m3=h*wanju(x1+h/2.0);
        k4=h*(z1+m3);
        m4=h*wanju(x1+h);
        y1+=(k1+2.0*k2+2.0*k3+k4)/6.0;
        z1+=(m1+2.0*m2+2.0*m3+m4)/6.0;
        x1+=h;

        bend[n]=y1/Ela/intia;

        Fsy[n]=neili(n*k);
        innerMz[n]=wanju(n*k);
        //f1<<x1<<"\t"<<y1<<"\n";
    }
    //f1.close();

}*/

double overHangingBeam::neili(const double &pos){
    double f=0;
    std::map<double,double>::iterator p=loads.end();
    for(std::map<double,double>::iterator iter=loads.begin() ;iter!=p;++iter)
    {
        if(iter->first<rRestraint){
            f+=(pos<rRestraint)?((pos<iter->first)?-(rRestraint-iter->first)*iter->second/rRestraint:
                (iter->first)*iter->second/rRestraint):0;}
        else
        {
            f+=(pos<iter->first)?((pos<rRestraint)?(iter->second*iter->first/rRestraint-iter->second):
                                                   -iter->second):0;
        }
    }
    p=meLoads.end();
    for(std::map<double,double>::iterator iter=meLoads.begin() ;iter!=p;++iter)
    {
        if(iter->first<rRestraint){
            f+=(pos<rRestraint)?iter->second/leng:0;
        }
        else{
            f+=(pos<rRestraint)?iter->second/leng:0;
        }
    }
    return f;
}

double overHangingBeam::bending(const double & pos){
#define force iter->second
#define ll rRestraint
#define aa iter->first
#define bb (ll-aa)
//#define loadPos iter->first
    double v=0;
    std::map<double,double>::iterator p=loads.end();
    for(std::map<double,double>::iterator iter=loads.begin() ;iter!=p;++iter)
    {
        if(iter->first<rRestraint){
            v+=(pos<ll)?((pos<aa)?-force*bb*pos/6.0/ll*(ll*ll-pos*pos-bb*bb):
                                           -force*bb/6.0/ll*(ll/bb*(pos-aa)*(pos-aa)*(pos-aa)+(ll*ll-bb*bb)*pos-pos*pos*pos)):
                                 (pos-ll)*(force*aa*bb*(ll+aa)/6.0/ll);
        }
#undef aa
#define aa (iter->first-ll)
        else{
            v+=(pos<iter->first)?((pos<ll)?force*aa*pos*(ll*ll-pos*pos)/6.0/ll:
                                           -force*(pos-ll)/6.0*(aa*(3.0*pos-ll)-(pos-ll)*(pos-ll))):
                                force*(-(pos-iter->first)*(aa/6.0)*(2.0*ll+3.0*aa)-aa*aa/3.0*(iter->first));
        }
    }
#undef force
#undef aa
#define me iter->second

    p=meLoads.end();
#define aa iter->first
    for(std::map<double,double>::iterator iter=meLoads.begin();iter!=p;++iter)
    {
        if(iter->first>rRestraint){
            v+=(pos<iter->first)?((pos<rRestraint)?me*(-pos*pos*pos/rRestraint+rRestraint*pos)/6.0:
                                                   -me*(pos*pos*.5-rRestraint*pos/3.0-rRestraint*rRestraint/6.0)):
                                    me*((-iter->first*iter->first*.5+rRestraint*iter->first/3.0+rRestraint*rRestraint/6.0)+
                                        (2.0*rRestraint/3.0-iter->first)*(pos-iter->first));
        }
        else{
            v+=(pos<rRestraint)?((pos<iter->first)?me*pos*(ll*ll-3.0*bb*bb-pos*pos)/ll:
                                                   me/ll*(-pos*pos*pos+3.0*ll*(pos-aa)*(pos-aa)+(ll*ll-3.0*bb*bb)*pos)):
                                                   (pos-ll)*me/6.0*(ll*ll-3*aa*aa);
        }
#undef me
#undef ll
#undef aa
#undef bb
    }
    return v/Ela/intia;
}
