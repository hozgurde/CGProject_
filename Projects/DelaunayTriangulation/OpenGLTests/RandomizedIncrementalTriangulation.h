#include <vector>
#include <limits>
#include "Eigen/Dense"
#include "Points.h"
#include "DataStructures/triangle.h"

class RandomizedIncrementalTriangulation
{
public:
    RandomizedIncrementalTriangulation(Points* random_points);
    ~RandomizedIncrementalTriangulation();

    void AdvanceTriangulation();
	void CompleteTriangulation();
	void Render(GLuint uniformMyColor);
    void UpdateBuffers();
    void InitializeBuffers();

	// All the random points initially given
    GLfloat *coordinates;

	// The current point index
	int curr_point_id;

	// The number of points
	int num_points;

    // Points of my triangulation
    std::vector<Point2Dd*> points; /**< vector of pointer of Point2Dd of the triangulation */

    // Triangles of my triangulation & Dag
    std::vector<Triangle*> triangles; /**< vector of pointer of Triangle, that represents the triangulation */

    // Nodes of my Dag
    std::vector<DagNode*> dagNodes; /**< vector of pointer of DagNode, that represents the Dag data structure */

    // Add a point inside my triangulation
    bool addPoint(const Point2Dd& p);

    // Check if a point lie in a line
    bool pointLieInALine(const Point2Dd& p, const Point2Dd& a, const Point2Dd& b);

    // Check if an Edge is legal
    void legalizeEdge(Point2Dd* pr, Point2Dd* pi, Point2Dd* pj, Triangle* tr);

    // Edge Flip of two triangle
    void edgeFlip(Triangle* tr1, Triangle* tr2, Point2Dd* pr, Point2Dd* pi, Point2Dd* pj);

    // Create a new triangle, set his dag node and his pointer of his dag node father
    Triangle* generateTriangle(Point2Dd* p, Point2Dd* p1, Point2Dd* p2, DagNode* dagNodeFather);

    // Create a new triangle, set his dag node and his two pointer of two dag node father
    Triangle* generateTriangle(Point2Dd* p, Point2Dd* p1, Point2Dd* p2, DagNode* dagNodeFather1, DagNode* dagNodeFather2);

    // Manage the triangulation if a point lie inside a triangle or in one of his edge
    void pointLieInsideTriangle(Point2Dd* pr, Triangle* triangleFather, DagNode* dagFather);
    void pointLieAB(Point2Dd* pr, Triangle* triangleFather, DagNode* dagFather);
    void pointLieBC(Point2Dd* pr, Triangle* triangleFather, DagNode* dagFather);
    void pointLieCA(Point2Dd* pr, Triangle* triangleFather, DagNode* dagFather);

    // Check Delaunay property
    bool isPointLyingInCircle(const Point2Dd &a, const Point2Dd &b, const Point2Dd &c, const Point2Dd &p,
                         bool includeEndpoints);
private:
    void RenderPrevPoints();
    void RenderNextPoints();
    void RenderTriangles();
    GLuint VAO, VBO, IBO;
    GLfloat* edges;
    int edges_size;
    int triangles_size;
};