#include <yarp/os/Property.h>
#include <iCub/ctrl/filters.h>
#include <yarp/math/Rand.h>
#include <yarp/os/Time.h>
#include <yarp/math/Math.h>

using namespace yarp::math;

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
		for(int j=0; j < sample_size; j++ )
		{
			u[j] = yarp::math::Rand::scalar();
		}

		for(int k=0; k < 2; k++ )
        {
            //trick to randomize the order of execution
            if( (k+i) % 2 == 0 )
            {
                tic = yarp::os::Time::now();
                y = old_filter.filt(u);
                toc = yarp::os::Time::now();

                old_time += (toc-tic);
            }
            else
            {

                tic = yarp::os::Time::now();
                rt_y = new_filter.filt(u);
                toc = yarp::os::Time::now();

                new_time += (toc-tic);
            }
        }
	}
	yarp::sig::Vector final_output_diff = rt_y-y;
    std::cout << "Final sample diff norm " << yarp::math::norm(final_output_diff) << std::endl;
	std::cout << "Time with old filter (us) " << 1e6*old_time/nr_of_samples << std::endl;
	std::cout << "Time with new implementation (us) " << 1e6*new_time/nr_of_samples << std::endl;
    std::cout << "Speedup : " << old_time/new_time << std::endl;

}

