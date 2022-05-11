#include "RandomizedIncrementalTriangulation.h"

using namespace std;

// Constructor
RandomizedIncrementalTriangulation::RandomizedIncrementalTriangulation(Points all_points) {

    this->coordinates = all_points.GetPoints();
    this->curr_point_id = 0;
    this->num_points = all_points.GetPointsSize();
}

void RandomizedIncrementalTriangulation::AdvanceTriangulation() {

    GLfloat x = this->coordinates[3 * curr_point_id];
    GLfloat y = this->coordinates[3 * curr_point_id + 1];
    Point2Dd p = Point2D<double>(x, y);
    addPoint(p);
    curr_point_id++;
}

void RandomizedIncrementalTriangulation::CompleteTriangulation() {

    void RandomizedIncrementalTriangulation::CompleteTriangulation() {

        for (int i = curr_point_id; i < num_points; ++i) {
            GLfloat x = coordinates[3 * i];
            GLfloat y = coordinates[3 * i + 1];
            Point2Dd p = Point2D<double>(x, y);
            addPoint(p);
        }
    }
}

bool RandomizedIncrementalTriangulation::addPoint(const Point2Dd &p) {
    DagNode *dagFather = Dag::navigate(dagNodes.front(), p);

    if (dagFather == nullptr) // Point already exist
        return false;

    points.push_back(new Point2Dd(p.x(), p.y()));

    Triangle *triangleFather = dagFather->getTriangle();

    // p lie on AB
    if (pointLieInALine(*points.back(), *triangleFather->getA(), *triangleFather->getB()))
        pointLieAB(points.back(), triangleFather, dagFather);
        // p lie on BC
    else if (pointLieInALine(*points.back(), *triangleFather->getB(), *triangleFather->getC()))
        pointLieBC(points.back(), triangleFather, dagFather);
        // p lie on CA
    else if (pointLieInALine(*points.back(), *triangleFather->getC(), *triangleFather->getA()))
        pointLieCA(points.back(), triangleFather, dagFather);
        // p lie inside the triangle
    else
        pointLieInsideTriangle(points.back(), triangleFather, dagFather);

    return true;
}

void RandomizedIncrementalTriangulation::legalizeEdge(Point2Dd* pr, Point2Dd* pi, Point2Dd* pj, Triangle* tr){

    Triangle* adjTriangle = nullptr;

    if(tr->getTriangleAdjacentA() != nullptr){
        if(Adjacencies::isAdjacenciesForTwoPoints(*tr->getTriangleAdjacentA(), *pi, *pj))
            adjTriangle = tr->getTriangleAdjacentA();
    }

    if(tr->getTriangleAdjacentB() != nullptr){
        if(Adjacencies::isAdjacenciesForTwoPoints(*tr->getTriangleAdjacentB(), *pi, *pj))
            adjTriangle = tr->getTriangleAdjacentB();
    }

    if(tr->getTriangleAdjacentC() != nullptr){
        if(Adjacencies::isAdjacenciesForTwoPoints(*tr->getTriangleAdjacentC(), *pi, *pj))
            adjTriangle = tr->getTriangleAdjacentC();
    }

    if(adjTriangle != nullptr){
        if(isPointLyingInCircle(*adjTriangle->getA(), *adjTriangle->getB(), *adjTriangle->getC(), *pr, false)){
            edgeFlip(tr, adjTriangle, pr, pi, pj);
        }
    }

}

void RandomizedIncrementalTriangulation::edgeFlip(Triangle* tr1, Triangle* tr2, Point2Dd* pr, Point2Dd* pi, Point2Dd* pj){
    tr1->setIsDeleted(true);
    tr2->setIsDeleted(true);

    Triangle* newTriangle1 = nullptr;
    Triangle* newTriangle2 = nullptr;

    Point2Dd* pk =  tr2->getThirdPoint(*pi, *pj);

    if(pk != nullptr){
        newTriangle1 = generateTriangle(pr, pi, pk, tr1->getDagNode(), tr2->getDagNode() );
        newTriangle2 = generateTriangle(pr, pk, pj, tr1->getDagNode(), tr2->getDagNode() );

        Adjacencies::setAdjacenciesAfterFlip(newTriangle1, newTriangle2, tr1, tr2);
        Adjacencies::setAdjacenciesAfterFlip(newTriangle2, newTriangle1, tr1, tr2);


        legalizeEdge(pr, pi, pk, newTriangle1);
        legalizeEdge(pr, pk, pj, newTriangle2);
    }
}

bool RandomizedIncrementalTriangulation::pointLieInALine(const Point2Dd &p, const Point2Dd &a, const Point2Dd &b) {
    double tolerance = 0.000001;
    double pab = a.dist(p) + p.dist(b);
    double ab = a.dist(b);

    return (pab - ab) < tolerance;
}


void RandomizedIncrementalTriangulation::pointLieInsideTriangle(Point2Dd* pr, Triangle* triangleFather, DagNode* dagFather){
    Triangle* tr1 = generateTriangle(pr, triangleFather->getA(), triangleFather->getB(), dagFather );
    Triangle* tr2 = generateTriangle(pr, triangleFather->getB(), triangleFather->getC(), dagFather );
    Triangle* tr3 = generateTriangle(pr, triangleFather->getC(), triangleFather->getA(), dagFather );

    triangleFather->setIsDeleted(true);

    Adjacencies::setAdjacencies(tr1, tr2, tr3, triangleFather);
    Adjacencies::setAdjacencies(tr2, tr3, tr1, triangleFather);
    Adjacencies::setAdjacencies(tr3, tr1, tr2, triangleFather);

    legalizeEdge( tr1->getA(), tr1->getB(), tr1->getC(), tr1 );
    legalizeEdge( tr2->getA(), tr2->getB(), tr2->getC(), tr2 );
    legalizeEdge( tr3->getA(), tr3->getB(), tr3->getC(), tr3 );
}

void RandomizedIncrementalTriangulation::pointLieAB(Point2Dd* pr, Triangle* triangleFather, DagNode* dagFather){

    Triangle* adjacentTriangle = Adjacencies::getTriangleAdjacentByTwoPoints(triangleFather, *triangleFather->getA(), *triangleFather->getB());

    DagNode* adjacentDagNode = adjacentTriangle->getDagNode();

    triangleFather->setIsDeleted(true);
    adjacentTriangle->setIsDeleted(true);

    Point2Dd* pl = adjacentTriangle->getThirdPoint(*triangleFather->getA(), *triangleFather->getB());

    Triangle* tr1 = generateTriangle(pr, triangleFather->getB(), triangleFather->getC(), dagFather);
    Triangle* tr2 = generateTriangle(pr, triangleFather->getC(), triangleFather->getA(), dagFather);
    Triangle* tr3 = generateTriangle(pr, triangleFather->getA(), pl, adjacentDagNode );
    Triangle* tr4 = generateTriangle(pr, pl, triangleFather->getB(), adjacentDagNode );


    Adjacencies::setAdjacencies(tr1, tr2, tr4, triangleFather);
    Adjacencies::setAdjacencies(tr2, tr3, tr1, triangleFather);
    Adjacencies::setAdjacencies(tr3, tr4, tr2, adjacentTriangle);
    Adjacencies::setAdjacencies(tr4, tr1, tr3, adjacentTriangle);

    legalizeEdge( tr4->getA(), tr4->getB(), tr4->getC(), tr4 );
    legalizeEdge( tr3->getA(), tr3->getB(), tr3->getC(), tr3 );
    legalizeEdge( tr2->getA(), tr2->getB(), tr2->getC(), tr2 );
    legalizeEdge( tr1->getA(), tr1->getB(), tr1->getC(), tr1 );
}

void RandomizedIncrementalTriangulation::pointLieBC(Point2Dd* pr, Triangle* triangleFather, DagNode* dagFather){
    Triangle* adjacentTriangle = Adjacencies::getTriangleAdjacentByTwoPoints(triangleFather, *triangleFather->getB(), *triangleFather->getC());

    DagNode* adjacentDagNode = adjacentTriangle->getDagNode();

    triangleFather->setIsDeleted(true);
    adjacentTriangle->setIsDeleted(true);

    Point2Dd* pl = adjacentTriangle->getThirdPoint(*triangleFather->getB(), *triangleFather->getC());

    Triangle* tr1 = generateTriangle(pr, triangleFather->getC(), triangleFather->getA(), dagFather);
    Triangle* tr2 = generateTriangle(pr, triangleFather->getA(), triangleFather->getB(), dagFather);
    Triangle* tr3 = generateTriangle(pr, triangleFather->getB(), pl, adjacentDagNode );
    Triangle* tr4 = generateTriangle(pr, pl, triangleFather->getC(), adjacentDagNode );

    Adjacencies::setAdjacencies(tr1, tr2, tr4, triangleFather);
    Adjacencies::setAdjacencies(tr2, tr3, tr1, triangleFather);
    Adjacencies::setAdjacencies(tr3, tr4, tr2, adjacentTriangle);
    Adjacencies::setAdjacencies(tr4, tr1, tr3, adjacentTriangle);

    legalizeEdge( tr4->getA(), tr4->getB(), tr4->getC(), tr4 );
    legalizeEdge( tr3->getA(), tr3->getB(), tr3->getC(), tr3 );
    legalizeEdge( tr2->getA(), tr2->getB(), tr2->getC(), tr2 );
    legalizeEdge( tr1->getA(), tr1->getB(), tr1->getC(), tr1 );
}

void RandomizedIncrementalTriangulation::pointLieCA(Point2Dd* pr, Triangle* triangleFather, DagNode* dagFather){
    Triangle* adjacentTriangle = Adjacencies::getTriangleAdjacentByTwoPoints(triangleFather, *triangleFather->getC(), *triangleFather->getA());

    DagNode* adjacentDagNode = adjacentTriangle->getDagNode();

    triangleFather->setIsDeleted(true);
    adjacentTriangle->setIsDeleted(true);

    Point2Dd* pl = adjacentTriangle->getThirdPoint(*triangleFather->getC(), *triangleFather->getA());

    Triangle* tr1 = generateTriangle(pr, triangleFather->getA(), triangleFather->getB(), dagFather);
    Triangle* tr2 = generateTriangle(pr, triangleFather->getB(), triangleFather->getC(), dagFather);
    Triangle* tr3 = generateTriangle(pr, triangleFather->getC(), pl, adjacentDagNode  );
    Triangle* tr4 = generateTriangle(pr, pl, triangleFather->getA(), adjacentDagNode );

    Adjacencies::setAdjacencies(tr1, tr2, tr4, triangleFather);
    Adjacencies::setAdjacencies(tr2, tr3, tr1, triangleFather);
    Adjacencies::setAdjacencies(tr3, tr4, tr2, adjacentTriangle);
    Adjacencies::setAdjacencies(tr4, tr1, tr3, adjacentTriangle);

    legalizeEdge( tr4->getA(), tr4->getB(), tr4->getC(), tr4 );
    legalizeEdge( tr3->getA(), tr3->getB(), tr3->getC(), tr3 );
    legalizeEdge( tr2->getA(), tr2->getB(), tr2->getC(), tr2 );
    legalizeEdge( tr1->getA(), tr1->getB(), tr1->getC(), tr1 );
}

bool RandomizedIncrementalTriangulation::isPointLyingInCircle(const Point2Dd& a, const Point2Dd& b, const Point2Dd& c, const Point2Dd& p, bool includeEndpoints) {
    Eigen::Matrix4d A;

    A << a.x(), a.y(), a.x()*a.x() + a.y()*a.y(), 1,
            b.x(), b.y(), b.x()*b.x() + b.y()*b.y(), 1,
            c.x(), c.y(), c.x()*c.x() + c.y()*c.y(), 1,
            p.x(), p.y(), p.x()*p.x() + p.y()*p.y(), 1;

    return (A.determinant() >= -std::numeric_limits<double>::epsilon());
}

Triangle* RandomizedIncrementalTriangulation::generateTriangle(Point2Dd* p,Point2Dd* p1, Point2Dd* p2, DagNode* dagNodeFather){
    this->triangles.push_back( new Triangle( p, p1, p2 ) );
    this->dagNodes.push_back( new DagNode( triangles.back() ) );

    triangles.back()->setDagNode( dagNodes.back() );

    Dag::addNode(dagNodes.back(), dagNodeFather);

    return triangles.back();
}

Triangle* RandomizedIncrementalTriangulation::generateTriangle(Point2Dd* p,Point2Dd* p1, Point2Dd* p2, DagNode* dagNodeFather1, DagNode* dagNodeFather2){
    this->triangles.push_back( new Triangle( p, p1, p2 ) );
    this->dagNodes.push_back( new DagNode( triangles.back() ) );
    triangles.back()->setDagNode( dagNodes.back() );

    Dag::addNode(dagNodes.back(), dagNodeFather1);
    Dag::addNode(dagNodes.back(), dagNodeFather2);

    return triangles.back();
}


int main() {

    return 0;
}