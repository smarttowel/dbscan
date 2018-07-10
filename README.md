Usage

```C++
//Suppose, we have set of points from OpenCV
std::vector<cv::Point2f> points;
//Define distance function as simple Euclidean distance between two points
auto distance = [](auto p1, auto p2){
    return std::hypot(p2.x - p1.x, p2.y - p1.y);
};
//Declare Dbscan instance
Dbscan<cv::Point2f> dbscan;
//Run algo with eps=50, minPts=10 and our distance function. 
//std::vector<std::vector<cv::Point2f>> returned as result
auto clusters = dbscan.run(points, 50, 10, distance);

```
