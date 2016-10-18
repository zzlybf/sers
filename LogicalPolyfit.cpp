/**
 * @file Logicalknn.cpp
 *
 * @brief Brute force k nearest neighbors.
 *
 * @par Synopsis: knn(A, k).
 *  
 * @par Summary:
 * Simple brute force k nearest neighbor enumeration for a full distance
 * matrix. When used without a generic matrix input, it simply identifies
 * the k smallest values per row.
 * <br>
 *
 * @par Input:
 * A is a 2-d full distance matrix with one double precision-valued attribute
 * chunked only along rows k number of nearest neighbors to identify
 * <br>
 *
 * @par Output array:
 * Has same schema as A, but is sparse with the k nearest neighbors
 * enumerated.
 * <br>
 *
 * @par Examples:
 * See help('knn')
 * <br>
 *
 * @author B. W. Lewis <blewis@paradigm4.com>
 */

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <query/Operator.h>
#include <log4cxx/logger.h>

using std::shared_ptr;
using boost::algorithm::trim;
using boost::starts_with;
using boost::lexical_cast;
using boost::bad_lexical_cast;

using namespace std;

namespace scidb
{

class LogicalPolyfit : public LogicalOperator
{
public:
    LogicalPolyfit(const string& logicalName, const string& alias):
        LogicalOperator(logicalName, alias)
    {
        ADD_PARAM_INPUT()
        ADD_PARAM_INPUT()
        ADD_PARAM_CONSTANT("int32")
        _usage = "polyfit(x, y, n)\n";}

/* inferSchema helps the query planner decide on the shape of
 * the output array. All operators must define this function.
 */
    ArrayDesc inferSchema(vector< ArrayDesc> schemas, shared_ptr< Query> query)
    {
        ArrayDesc const& matrix1 = schemas[0];
        if(matrix1.getAttributes(true)[0].getType() != TID_DOUBLE)
           throw SYSTEM_EXCEPTION(SCIDB_SE_INTERNAL, SCIDB_LE_ILLEGAL_OPERATION) <<  "polyfit first argument requires a single double precision-valued attribute";
        if(matrix1.getDimensions().size() != 1 )
           throw SYSTEM_EXCEPTION(SCIDB_SE_INTERNAL, SCIDB_LE_ILLEGAL_OPERATION) <<  "polyfit first argument requires an array input";
        if (matrix1.getDimensions()[0].getChunkInterval() != static_cast<int64_t>(matrix1.getDimensions()[0].getLength()))
            throw SYSTEM_EXCEPTION(SCIDB_SE_INTERNAL, SCIDB_LE_ILLEGAL_OPERATION) << "polyfit first argument does not accept column partitioning of the input array, use repart first";        
        ArrayDesc const& matrix2 = schemas[1];
        if(matrix2.getAttributes(true)[0].getType() != TID_DOUBLE)
           throw SYSTEM_EXCEPTION(SCIDB_SE_INTERNAL, SCIDB_LE_ILLEGAL_OPERATION) <<  "polyfit second argument requires a single double precision-valued attribute";
        if(matrix2.getDimensions().size() != 1 )
           throw SYSTEM_EXCEPTION(SCIDB_SE_INTERNAL, SCIDB_LE_ILLEGAL_OPERATION) <<  "polyfit second argument requires an array input";
        if (matrix2.getDimensions()[0].getChunkInterval() != static_cast<int64_t>(matrix2.getDimensions()[0].getLength()))
            throw SYSTEM_EXCEPTION(SCIDB_SE_INTERNAL, SCIDB_LE_ILLEGAL_OPERATION) << "polyfit second argument does not accept column partitioning of the input array, use repart first";
                
        return ArrayDesc("polyfit_array", matrix1.getAttributes(), matrix1.getDimensions(), matrix1.getDistribution(),
                query->getDefaultArrayResidency());
    }
};

REGISTER_LOGICAL_OPERATOR_FACTORY(LogicalPolyfit, "polyfit");

}
