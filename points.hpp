#ifndef points_hpp
#define points_hpp

class TSP_Points {
    public:
        TSP_Points(const std::vector<std::pair<int, int>> points);
        const std::vector<std::pair<int, int>> points;
};
#endif