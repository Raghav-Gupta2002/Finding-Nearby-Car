#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <random>
#include "color.hpp"

using namespace std;

static float lat1d = 12.9611159;

// Longitude of customer who needs a cab.
static float lon1d = 77.6362214;

#define pi 3.14159265358979323846
#define earth_radius 6371.0

std::vector<std::pair<std::string,float>> result;

class json
{
public:
	// json();

	double degtorad(double deg)
	{
    	return ( deg * pi / 180);
	}

	double distanceEarth(double lat2d, double lon2d)
    {
	    double lat1, lon1, lat2, lon2,
	           delta_lon, central_ang;

	    lat1 = degtorad(lat1d);
	    lon1 = degtorad(lon1d);
	    lat2 = degtorad(lat2d);
	    lon2 = degtorad(lon2d);
	  
	    delta_lon = lon2 - lon1;
	      
	    // great circle distance formula. 
	    central_ang = acos ( sin(lat1) * 
	                  sin(lat2) + cos(lat1) * 
	                  cos(lat2) * cos(delta_lon) );  
	                    
	    return (earth_radius * central_ang); 
	} 

	void json_parser(){
        ifstream customer_list ("customers.json");
        if (customer_list.is_open())
		{

			while (getline(customer_list, line))
			{

                f = 0; x = 0; y = 0; fi = 0; m = 0, n = 0;
                length = line.size();

                for (j = 0; j < length; j++)
				{

					if (line[j] == '"')
						f++;

                    else if (line[j] == ':')
                        fi++;

                    // To get latitude of the location.
					if (f == 3)
					{
                        j++;

                        while (line[j] != '"')
						{
                            latitude_as_string[x] = line[j];
                            x++; j++;
						}

                        j--; latitude_as_string[x] = '\0';
                    }

                    // To get longitude of the location.
                    else if (f == 13)
					{
                        j++;

                        while (line[j] != '"')
						{
                            longitude_as_string[y] = line[j];
                            y++; j++;
                        }

                        j--; longitude_as_string[y] = '\0';
					}

                    // To get id of the friend.
                    if (fi == 2)
					{
                        j += 2;

                        while (line[j] != ',')
						{
                            id_as_string[m] = line[j];
                            m++; j++;
						}

                        j--; id_as_string[m] = '\0';
                        fi++;
					}

                    // To get name of the friend.
                    else if (fi == 4)
					{
                        j += 2;

                        while (line[j] != ',')
						{
                            name[n] = line[j];
                            n++; j++;
                        }

                        j--; name[n] = '\0';
                        fi++; f += 2;
                    }
                }

                // Converting latitude and longitude
                // in string to float.
                lat2d = atof(latitude_as_string);
                lon2d = atof(longitude_as_string);
                distance_calculator();
            }
        }
          
        // closing stream of customer's file. 
        customer_list.close();
          
        // closing stream of answer's file. 
        // out.close();
	};
	

	void distance_calculator()
    {
        if (distanceEarth(lat2d, lon2d) <= 50.0000)
        {
            // Converting id to int format.
            id[i] = atoll(id_as_string);
            i++;
            result.push_back(std::make_pair(name, id[i-1]));
        }
    }
private:
	long long int length, i, j, x, y, m, 
                  n, f, fi, id[100000];

    char latitude_as_string[1000],
         longitude_as_string[1000],
         id_as_string[1000], name[1000];

    double lat2d, lon2d;
	string line;
};

class person : public json
{
public:

    static bool continue_cab(){
        char yN;
        cout<<yellow_fg<<"\n\n########\tWant to ride ? !!!                   \t\t########"<<reset<<endl;
        cout<<yellow_fg<<"########\tEnter Y/y to ride and n/N to leave!!!\t\t########"<<reset<<endl;
        cout<<"\t\t";
        cin>>yN;
        if(yN == 'y' || yN == 'Y')
            return true;
        else
            return false;
    }
    int random_num_generator(){
        std::random_device rd;
        std::mt19937 rng(rd());
        std::uniform_int_distribution<int> random(2, 4);
        return random(rng);
    }

    static bool available(int id){
        auto it = mp.find(id);
        if(it == mp.end() || it->second <= 0)
            return true;
        else
            return false;
    }

    void push(int id, int count){
        mp[id] = count;
    }

    void person_coordinate(){
        cout<<red_fg<<"\n\tEnter latitude of the person : "<<reset<<endl<<"\t";
        cin>>lat1d;
        cout<<red_fg<<"\n\tEnter longitude of the person : "<<reset<<endl<<"\t";
        cin>>lon1d;
    }

    static void reduce_count(){
        if(!mp.empty()){
            for(auto& x: mp){
                if(x.second == 0){
                    mp.erase(x.first);
                }
                if(x.second != 0){
                    x.second--;
                }
                // cout<<"x : "<<x.second<<endl;

            }
        }
    }

private:
    static std::unordered_map<int, int> mp;
};

unordered_map<int , int> person::mp;

int print_result(){
    int ride = 0;
    std::vector<std::pair<string,float>> v;
    for(auto & x:result){
        if(person::available(x.second)){
            cout<<green_fg<<"\tNAME : "<<x.first<<"-->"<<x.second<<"(user_id)"<<reset<<endl;
            v.push_back(std::make_pair(x.first,x.second));
            ride++;
        }else{
            continue;
        }
    }
    v.shrink_to_fit();
    result = v;
    cout<<green_fg<<"\n\tTOTAL NUMBER OF AVAILABLE CABS ARE : "<<ride<<reset<<endl;
    return ride;
}

int main()
{
    int ride_num = 1;
    int not_avail_id = 0;
    bool flag = true;

    while(person::continue_cab()){
        person::reduce_count();
        result.clear();
        cout<<green_fg<<"\n########\tHELLO, WELCOME TO RIDE\t#########\n"<<reset<<endl;
        cout<<green_fg<<"\tRide Number(Person Number) : "<<ride_num<<reset<<endl;
        // creating person object
        person obj;
        obj.person_coordinate();
        obj.json_parser();
        cout<<magenta_fg<<"\n\tNUMBER OF AVAILABLE CABS\n"<<reset<<endl;

        if(!result.empty()){
            if(print_result()){
                int x = obj.random_num_generator();

                obj.push(result[0].second, x);

                cout<<blue_fg<<"\n\tCab with "<<yellow_fg<<"name : "<<result[0].first<<blue_fg<<" and "<<yellow_fg<<"user id : "<<result[0].second
                    <<blue_fg<<" is selected for the ride.\n\t"<<"HE/SHE WILL BE AVAILABLE AGAIN AFTER "<<yellow_fg<<x-1
                    <<" rides."<<reset<<endl;

            }else{
                cout<<red_fg<<"\n\n\tNO CABS ARE AVAILABLE IN THAT AREA.\n\n"<<reset<<endl;
            }
        }else{
            cout<<red_fg<<"\n\n\tNO CABS ARE AVAILABLE IN THAT AREA.\n\n"<<reset<<endl;
        }
        ride_num++;
    }

    return 0;
}