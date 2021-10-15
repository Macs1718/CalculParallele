# include <vector>
# include <cassert>
# include <string>
# include <iostream>
# include <algorithm>
# include <chrono>
#include  <thread>
void dot_part( const std::vector<double>& u, const std::vector<double>& v, 
            int deb, int fin, double& scal )
{
  assert(u.size() == v.size());
  fin = std::min(fin, int(u.size()));
  scal = 0.;
  for ( int i = deb; i < fin; ++i )
    scal += u[i]*v[i];
}
double dot( std::vector<double>& u, std::vector<double>& v )
{
  int num_threads = 4;//std::thread::hardware_concurrency();
  std::vector<std::thread> threads; threads.reserve(num_threads);
  std::vector<double> partial_sums(num_threads);
  std::size_t dim_loc = u.size()/num_threads;
  int reste = u.size()%num_threads;
  for ( int it = 0; it < num_threads; ++it ) {
    int deb, fin;
    if (it < reste)
    {
      deb = it * (dim_loc+1); fin = (it+1)*(dim_loc+1);
    }
    else
    {
      deb = it*dim_loc + reste; fin = (it+1)*dim_loc + reste;
    }
    threads.emplace_back(dot_part, std::cref(u), std::cref(v), deb, fin, std::ref(partial_sums[it]) );
  }
  for ( auto& th : threads ) th.join();
  double global_sum = 0.;
  for ( int it = 0; it < num_threads; ++it )
    global_sum += partial_sums[it];
  return global_sum;
}

int main( int nargs, char* vargs[])
{
  std::chrono::time_point<std::chrono::system_clock> start, end;
  int N = 100023;
  int nbSamples = 1024;
  if (nargs > 1) {
    nbSamples = std::stoi(vargs[1]);
  }
  
  start = std::chrono::system_clock::now();
  std::vector<std::vector<double>> U(nbSamples), V(nbSamples);
  for ( int iSample = 0; iSample < nbSamples; ++iSample ) {
    U[iSample] = std::vector<double>(N);
    V[iSample] = std::vector<double>(N);
    for ( int i = 0; i < N; ++i ) {
      U[iSample][i] = (iSample + 1 + i)%N;
      V[iSample][i] = (nbSamples - iSample + i)%N;
    }
  }
  end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end-start;
  std::cout << "Temps assemblage vecteurs : " << elapsed_seconds.count() 
              << std::endl;

  start = std::chrono::system_clock::now();
  std::vector<double> result(nbSamples);
  for ( int iSample = 0; iSample < nbSamples; ++iSample )
    result[iSample] = dot(U[iSample],V[iSample]);
  end = std::chrono::system_clock::now();
  elapsed_seconds = end-start;
  std::cout << "Temps produits scalaires : " << elapsed_seconds.count() 
              << std::endl;

  start = std::chrono::system_clock::now();
  double ref = result[0];
  double sum = 0;;
  for ( const auto& val : result )
    sum += val;
  sum /= ref;
  end = std::chrono::system_clock::now();
  elapsed_seconds = end-start;
  std::cout << "Temps sommation : " << elapsed_seconds.count() 
              << std::endl;
  std::cout << "sum : " << sum << std::endl;
  return 0;
}
