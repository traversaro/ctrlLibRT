#include <yarp/os/Property.h>
#include <iCub/ctrl/filters.h>
#include <yarp/math/Rand.h>
#include <yarp/os/Time.h>
#include <yarp/math/Math.h>

#include "filters.h"

/*******************************************************/
int main(int argc, char *argv[])
{
	yarp::os::Property opt;
	opt.fromCommand(argc,argv);

	int coeff_size = opt.check("coeff_size",10,"Size of the coeffient vectors").asInt();
	int nr_of_samples = opt.check("nr_of_samples",1000,"Nr of samples to filter").asInt();
	int sample_size = opt.check("sample_size",25,"Size of the sample to filter").asInt();

	yarp::sig::Vector num(coeff_size), den(coeff_size);
	yarp::sig::Vector rt_y(sample_size), y(sample_size), u(sample_size);

	for(int i=0; i < coeff_size; i++ )
	{
		num[i] = yarp::math::Rand::scalar();
		den[i] = yarp::math::Rand::scalar();
	}

	for(int i=0; i < sample_size; i++ )
	{
		rt_y[i] = y[i] = yarp::math::Rand::scalar();
		u[i] = yarp::math::Rand::scalar();
	}

	iCub::ctrl::Filter old_filter(num,den,y);
	iCub::ctrl::realTime::Filter new_filter(num,den,rt_y);

	double old_time = 0.0;
	double new_time = 0.0;

    double tic,toc;
	for(int i = 0; i < nr_of_samples; i++ )
	{
		for(int i=0; i < sample_size; i++ )
		{
			u[i] = yarp::math::Rand::scalar();
		}

	    tic = yarp::os::Time::now();
	    y = old_filter.filt(u);
	    toc = yarp::os::Time::now();

        old_time += (toc-tic);

	    tic = yarp::os::Time::now();
	    rt_y = new_filter.filt(u);
	    toc = yarp::os::Time::now();

        new_time += (toc-tic);
	}
	std::cout << "Time with old filter " << old_time << std::endl;
	std::cout << "Time with new implementation " << new_time << std::endl;

}

