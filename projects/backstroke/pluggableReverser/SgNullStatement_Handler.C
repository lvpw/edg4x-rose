#include "SgNullStatement_Handler.h"
#include <boost/tuple/tuple.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>

#define foreach BOOST_FOREACH
#define reverse_foreach BOOST_REVERSE_FOREACH

using namespace std;

StatementReversal SgNullStatement_Handler::generateReverseAST(SgStatement* stmt, const EvaluationResult& evaluationResult)
{
	ROSE_ASSERT(evaluationResult.getStatementHandler() == this && evaluationResult.getChildResults().empty());
	return StatementReversal(NULL, NULL);
}

EvaluationResult SgNullStatement_Handler::evaluate(SgStatement* stmt, const VariableVersionTable& var_table)
{
	EvaluationResult result;
	if (isSgNullStatement(stmt))
	{
		result = EvaluationResult(this, stmt, var_table);
	}
	return result;
}