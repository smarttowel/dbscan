#ifndef DBSCAN_H
#define DBSCAN_H

#include <vector>
#include <algorithm>

template<typename T>
class Dbscan
{
public:
    enum {
        Noise = -2,
        Undefined = -1
    };
    using DistanceFunction = std::function<float(const T&, const T&)>;
    std::vector<std::vector<T> > run(const std::vector<T> &points, float eps, size_t minPts, DistanceFunction d);

private:
    std::vector<size_t> rangeQuery(const std::vector<T> &db, const T &point, float eps, DistanceFunction d);
};

template<typename T>
std::vector<std::vector<T>> Dbscan<T>::run(const std::vector<T> &points, float eps, size_t minPts, DistanceFunction d)
{
    std::vector<int> cluster(points.size(), Undefined);

    int currentClusterId = 0;
    for(size_t i = 0; i < points.size(); i++)
    {
        if(cluster[i] == Undefined)
        {
            std::vector<size_t> neighbors = rangeQuery(points, points[i], eps, d);
            if(neighbors.size() >= minPts)
            {
                cluster[i] = currentClusterId;

                neighbors.erase(std::remove(neighbors.begin(), neighbors.end(), i));

                for(size_t j = 0; j < neighbors.size(); j++)
                {
                    size_t idx = neighbors[j];
                    int &clusterId = cluster[idx];
                    if(clusterId == Noise)
                        clusterId = currentClusterId;
                    if(clusterId == Undefined)
                    {
                        clusterId = currentClusterId;

                        std::vector<size_t> neighbors2 = rangeQuery(points, points[idx], eps, d);
                        if(!neighbors2.empty() && neighbors2.size() >= minPts)
                            neighbors.insert(neighbors.end(), neighbors2.begin(), neighbors2.end());
                    }
                }
                currentClusterId++;
            }
            else
            {
                cluster[i] = Noise;
            }
        }
    }

    std::vector<std::vector<T>> result(currentClusterId);
    for(size_t j = 0; j < points.size(); j++)
    {
        int clusterId = cluster[j];
        if(clusterId >= 0)
            result[clusterId].push_back(points[j]);
    }

    return result;
}

template<typename T>
std::vector<size_t> Dbscan<T>::rangeQuery(const std::vector<T> &db, const T &point, float eps, DistanceFunction d)
{
    std::vector<size_t> result;
    result.reserve(db.size());
    for(size_t i = 0; i < db.size(); i++)
        if(d(point, db[i]) <= eps)
            result.push_back(i);
    return result;
}

#endif // DBSCAN_H
