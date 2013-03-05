#include "regex.h"
#include<iostream>
#include<fstream>
#include "symbol_table.cpp"
using namespace std;

class_regex::class_regex()
{
	m_nNextStateID	= 0;
}

class_regex::~class_regex()
{
	// Clean up all allocated memory
	CleanUp();
}

bool class_regex::evaluate()
{
	// First pop the operator from the stack
	if(m_OperatorStack.size()>0)
	{
		char chOperator = m_OperatorStack.top();
		m_OperatorStack.pop();

		// Check which operator it is
		switch(chOperator)
		{
			case  42:
				return star();
				break;
			case 124:
				return Union();
				break;
			case   8:
				return concat();
				break;
		}

		return false;
	}

	return false;
}

bool class_regex::concat()
{
	// Pop 2 elements
	FSA_TABLE A, B;
	if(!Pop(B) || !Pop(A))
		return false;

	// Now evaluate AB
	// Basically take the last state from A
	// and add an epsilon transition to the
	// first state of B. Store the result into
	// new NFA_TABLE and push it onto the stack
	A[A.size()-1]->AddTransition(0, B[0]);
	A.insert(A.end(), B.begin(), B.end());

	// Push the result onto the stack
	m_OperandStack.push(A);

	//TRACE("CONCAT\n");

	return true;
}

bool class_regex::star()
{
	// Pop 1 element
	FSA_TABLE A, B;
	if(!Pop(A))
		return false;

	// Now evaluate A*
	// Create 2 new states which will be inserted 
	// at each end of deque. Also take A and make 
	// a epsilon transition from last to the first 
	// state in the queue. Add epsilon transition 
	// between two new states so that the one inserted 
	// at the begin will be the source and the one
	// inserted at the end will be the destination
	class_state *pStartState	= new class_state(++m_nNextStateID);
	class_state *pEndState	= new class_state(++m_nNextStateID);
	pStartState->AddTransition(0, pEndState);

	// add epsilon transition from start state to the first state of A
	pStartState->AddTransition(0, A[0]);

	// add epsilon transition from A last state to end state
	A[A.size()-1]->AddTransition(0, pEndState);

	// From A last to A first state
	A[A.size()-1]->AddTransition(0, A[0]);

	// construct new DFA and store it onto the stack
	A.push_back(pEndState);
	A.push_front(pStartState);

	// Push the result onto the stack
	m_OperandStack.push(A);

	//TRACE("STAR\n");

	return true;
}

bool class_regex::Union()
{
	// Pop 2 elements
	FSA_TABLE A, B;
	if(!Pop(B) || !Pop(A))
		return false;

	// Now evaluate A|B
	// Create 2 new states, a start state and
	// a end state. Create epsilon transition from
	// start state to the start states of A and B
	// Create epsilon transition from the end 
	// states of A and B to the new end state
	class_state *pStartState	= new class_state(++m_nNextStateID);
	class_state *pEndState	= new class_state(++m_nNextStateID);
	pStartState->AddTransition(0, A[0]);
	pStartState->AddTransition(0, B[0]);
	A[A.size()-1]->AddTransition(0, pEndState);
	B[B.size()-1]->AddTransition(0, pEndState);

	// Create new NFA from A
	B.push_back(pEndState);
	A.push_front(pStartState);
	A.insert(A.end(), B.begin(), B.end());

	// Push the result onto the stack
	m_OperandStack.push(A);

	//TRACE("UNION\n");

	return true;
}

string class_regex::concat_expand(string strRegEx)
{
	string strRes;

	for(int i=0; i<strRegEx.size()-1; ++i)
	{
		char cLeft	= strRegEx[i];
		char cRight = strRegEx[i+1];
		strRes	   += cLeft;
		if((IsInput(cLeft)) || (IsRightParanthesis(cLeft)) || (cLeft == '*'))
			if((IsInput(cRight)) || (IsLeftParanthesis(cRight)))
				strRes += char(8);
	}
	strRes += strRegEx[strRegEx.size()-1];

	return strRes;
}

bool class_regex::create_nfa(string strRegEx)
{
	// Parse regular expresion using similar 
	// method to evaluate arithmetic expressions
	// But first we will detect concatenation and
	// insert char(8) at the position where 
	// concatenation needs to occur
	strRegEx = concat_expand(strRegEx);

	for(int i=0; i<strRegEx.size(); ++i)
	{
		// get the charcter
		char c = strRegEx[i];

		if(IsInput(c))
			Push(c);
		else if(m_OperatorStack.empty())
			m_OperatorStack.push(c);
		else if(IsLeftParanthesis(c))
			m_OperatorStack.push(c);
		else if(IsRightParanthesis(c))
		{
			// Evaluate everyting in paranthesis
			while(!IsLeftParanthesis(m_OperatorStack.top()))
				if(!evaluate())
					return false;
			// Remove left paranthesis after the evaluation
			m_OperatorStack.pop(); 
		}
		else
		{
			while(!m_OperatorStack.empty() && Presedence(c, m_OperatorStack.top()))
				if(!evaluate())
					return false;
			m_OperatorStack.push(c);
		}
	}

	// Evaluate the rest of operators
	while(!m_OperatorStack.empty())
		if(!evaluate())
			return false;

	// Pop the result from the stack
	if(!Pop(m_NFATable))
		return false;

	// Last NFA state is always accepting state
	m_NFATable[m_NFATable.size()-1]->m_bAcceptingState = true;

	return true;
}

void class_regex::Push(char chInput)
{
	// Create 2 new states on the heap
	class_state *s0 = new class_state(++m_nNextStateID);
	class_state *s1 = new class_state(++m_nNextStateID);

	// Add the transition from s0->s1 on input character
	s0->AddTransition(chInput, s1);

	// Create a NFA from these 2 states 
	FSA_TABLE NFATable;
	NFATable.push_back(s0);
	NFATable.push_back(s1);

	// push it onto the operand stack
	m_OperandStack.push(NFATable);

	// Add this character to the input character set
	m_InputSet.insert(chInput);

	//TRACE("PUSH %s\n", CString(chInput));
}

bool class_regex::Pop(FSA_TABLE &NFATable)
{
	// If the stack is empty we cannot pop anything
	if(m_OperandStack.size()>0)
	{
		NFATable = m_OperandStack.top();
		m_OperandStack.pop();
		return true;
	}

	return false;
}

void class_regex::epsilon_closure(set<class_state*> T, set<class_state*> &Res)
{
	Res.clear();

	// Initialize result with T because each state
	// has epsilon closure to itself
	Res = T;

	// Push all states onto the stack
	stack<class_state*> unprocessedStack;
	set<class_state*>::iterator iter;
	for(iter=T.begin(); iter!=T.end(); ++iter)
		unprocessedStack.push(*iter);

	// While the unprocessed stack is not empty
	while(!unprocessedStack.empty())
	{
		// Pop t, the top element from unprocessed stack
		class_state* t = unprocessedStack.top();
		unprocessedStack.pop();

		// Get all epsilon transition for this state
		vector<class_state*> epsilonStates;
		t->GetTransition(0, epsilonStates);

		// For each state u with an edge from t to u labeled epsilon
		for(int i=0; i<epsilonStates.size(); ++i)
		{
			class_state* u = epsilonStates[i];
			// if u not in e-closure(T)
			if(Res.find(u) == Res.end())
			{
				Res.insert(u);
				unprocessedStack.push(u);
			}
		}
	}
}

void class_regex::move(char chInput, set<class_state*> T, set<class_state*> &Res)
{
	Res.clear();

	/* This is very simple since I designed the NFA table
	   structure in a way that we just need to loop through
	   each state in T and recieve the transition on chInput.
	   Then we will put all the results into the set, which
	   will eliminate duplicates automatically for us.
	   */
	set<class_state*>::iterator iter;
	for(iter=T.begin(); iter!=T.end(); ++iter)
	{
		// Get all transition states from this specific
		// state to other states
		class_state* pState = *iter;
		vector<class_state*> States;
		pState->GetTransition(chInput, States);

		// Now add these all states to the result
		// This will eliminate duplicates
		for(int i=0; i<States.size(); ++i)
			Res.insert(States[i]);
	}
}

void class_regex::convert_nfa_to_dfa()
{
	// Clean up the DFA Table first
	for(int i=0; i<m_DFATable.size(); ++i)
		delete m_DFATable[i];
	m_DFATable.clear();

	// Check is NFA table empty
	if(m_NFATable.size() == 0)
		return;

	// Reset the state id for new naming
	m_nNextStateID = 0;

	// Array of unprocessed DFA states
	vector<class_state*> unmarkedStates;

	// Starting state of DFA is epsilon closure of 
	// starting state of NFA state (set of states)
	set<class_state*> DFAStartStateSet;
	set<class_state*> NFAStartStateSet;
	NFAStartStateSet.insert(m_NFATable[0]);
	epsilon_closure(NFAStartStateSet, DFAStartStateSet);

	// Create new DFA State (start state) from the NFA states
	class_state *DFAStartState = new class_state(DFAStartStateSet, ++m_nNextStateID);

	// Add the start state to the DFA
	m_DFATable.push_back(DFAStartState);

	// Add the starting state to set of unprocessed DFA states
	unmarkedStates.push_back(DFAStartState);
	while(!unmarkedStates.empty())
	{
		// process an unprocessed state
		class_state* processingDFAState = unmarkedStates[unmarkedStates.size()-1];
		unmarkedStates.pop_back();

		// for each input signal a
		set<char>::iterator iter;
		for(iter=m_InputSet.begin(); iter!=m_InputSet.end(); ++iter)
		{
			set<class_state*> MoveRes, EpsilonClosureRes;
			move(*iter, processingDFAState->GetNFAState(), MoveRes);
			epsilon_closure(MoveRes, EpsilonClosureRes);

			// Check is the resulting set (EpsilonClosureSet) in the
			// set of DFA states (is any DFA state already constructed
			// from this set of NFA states) or in pseudocode:
			// is U in D-States already (U = EpsilonClosureSet)
			bool bFound		= false;
			class_state *s	= NULL;
			for(int i=0; i<m_DFATable.size(); ++i)
			{
				s = m_DFATable[i];
				if(s->GetNFAState() == EpsilonClosureRes)
				{
					bFound = true;
					break;
				}
			}
			if(!bFound)
			{
				class_state* U = new class_state(EpsilonClosureRes, ++m_nNextStateID);
				unmarkedStates.push_back(U);
				m_DFATable.push_back(U);

				// Add transition from processingDFAState to new state on the current character
				processingDFAState->AddTransition(*iter, U);
			}
			else
			{
				// This state already exists so add transition from 
				// processingState to already processed state
				processingDFAState->AddTransition(*iter, s);
			}
		}
	}
}

void class_regex::reduce_dfa()
{
	// Get the set of all dead end states in DFA
	set<class_state*> DeadEndSet;
	for(int i=0; i<m_DFATable.size(); ++i)
		if(m_DFATable[i]->IsDeadEnd())
			DeadEndSet.insert(m_DFATable[i]);

	// If there are no dead ends then there is nothing to reduce
	if(DeadEndSet.empty())
		return;

	// Remove all transitions to these states
	set<class_state*>::iterator iter;
	for(iter=DeadEndSet.begin(); iter!=DeadEndSet.end(); ++iter)
	{
		// Remove all transitions to this state
		for(int i=0; i<m_DFATable.size(); ++i)
			m_DFATable[i]->RemoveTransition(*iter);

		// Remove this state from the DFA Table
		deque<class_state*>::iterator pos;
		for(pos=m_DFATable.begin(); pos!=m_DFATable.end(); ++pos)
			if(*pos == *iter)
				break;
		// Erase element from the table
		m_DFATable.erase(pos);

		// Now free the memory used by the element
		delete *iter;
	}
}

bool class_regex::SetRegExLine(string strRegEx)
{
	// 1. Clean up old regular expression
	CleanUp();

	// 2. Create NFA
	if(!create_nfa(strRegEx))
		return false;

	closureall();
	// 3. Convert to DFA
	convert_nfa_to_dfa();

	// 4. Reduce DFA
	reduce_dfa();

	//Create and save transition table
	save_dfa_table();

	return true;
}

void class_regex::closureall()
{
	eclose.clear();
	eclose_state.clear();
	class_state *finState;
	for(int i=0; i<m_NFATable.size(); ++i)
	{
		class_state *pState = m_NFATable[i];
		set<class_state*> eclosure,setpState;
		setpState.insert(pState);

		if(pState->m_bAcceptingState)
			finState = pState;

		//find eclosure of all states
		epsilon_closure(setpState,eclosure);
		eclose.push_back(eclosure);
		eclose_state.push_back(pState);
	}

	vector< set<class_state*> >::iterator iter_vect;
	set<class_state*>::iterator iter_set;
	/*for(iter_vect=eclose.begin();iter_vect!=eclose.end();iter_vect++)
	{
		for(iter_set=(*iter_vect).begin();iter_set!=(*iter_vect).end();iter_set++)
		{
			cout << (*iter_set)->GetStateID() << " -->> ";
		}
		cout << endl;
	}*/
	vector<string> fin_eclose;
	vector<string>::iterator iter_fin_eclose;
	vector<class_state*>::iterator iter_state = eclose_state.begin();
	for(iter_vect=eclose.begin();iter_vect!=eclose.end();iter_vect++)
	{
		for(iter_set=(*iter_vect).begin();iter_set!=(*iter_vect).end();iter_set++)
		{
			if((*iter_set)==finState)
			{
				string strState = (*iter_state)->GetStateID();
				if((*iter_state)->m_bAcceptingState)
						{strState.erase(strState.find_first_of("{"),1);strState.erase(strState.find_last_of("}"),1);}
				fin_eclose.push_back(strState);
				break;
			}
		}
		iter_state++;
	}
	/*for(iter_fin_eclose=fin_eclose.begin();iter_fin_eclose!=fin_eclose.end();iter_fin_eclose++)
		;//cout << *iter_fin_eclose << " -- ";
	cout << endl;*/

	vector< vector<string> >::iterator iter_accept;
	vector<string>::iterator iter_accept_str;
	if(count_accept_state_nfa == 0) accept_state_nfa.push_back(fin_eclose);
	else
	{
		for(iter_accept=accept_state_nfa.begin();iter_accept!=accept_state_nfa.end();iter_accept++)
		{
			for(iter_fin_eclose=fin_eclose.begin();iter_fin_eclose!=fin_eclose.end();iter_fin_eclose++)
			{
				for(iter_accept_str=(*iter_accept).begin();iter_accept_str!=(*iter_accept).end();iter_accept_str++)
				{
					if((*iter_fin_eclose) == (*iter_accept_str))
					{
						fin_eclose.erase(iter_fin_eclose);
					}
				}
			}
		}
		accept_state_nfa.push_back(fin_eclose);
	}
	/*for(iter_fin_eclose=fin_eclose.begin();iter_fin_eclose!=fin_eclose.end();iter_fin_eclose++)
		cout << *iter_fin_eclose << " -- ";
	cout << endl;*/
	count_accept_state_nfa++;
}

void class_regex::RegExFin()
{

	set<class_state*> NfaStates;
	set<class_state*>::iterator it;

	vector<string> fin_eclose;
	vector<string>::iterator iter_fin_eclose;

	vector<class_state*> acceptDfaStates;
	vector<class_state*>::iterator iter_acceptDfa;

	vector<string> acceptDfaStates_str;
	vector<string>::iterator iter_acceptDfa_str;
	//int count_int = 0;
	for(int i=0; i<m_DFATable.size(); ++i)
	{
		class_state *pState = m_DFATable[i];
		
		if(pState->m_bAcceptingState)
		{
			string strState = pState->GetStateID();
			acceptDfaStates.push_back(pState);
			if(pState->m_bAcceptingState)
						{strState.erase(strState.find_first_of("{"),1);strState.erase(strState.find_last_of("}"),1);}
			acceptDfaStates_str.push_back(strState);		
		}
	}

	vector< vector<string> >::iterator iter_accept;
	vector<string>::iterator iter_accept_str;

	vector<int> accept_state_dfa_temp;
	int tobreak;
	for(iter_accept=accept_state_nfa.begin();iter_accept!=accept_state_nfa.end();iter_accept++)
	{
		accept_state_dfa_temp.clear();
		for(iter_accept_str=(*iter_accept).begin();iter_accept_str!=(*iter_accept).end();iter_accept_str++)
		{
			
			for(iter_acceptDfa=acceptDfaStates.begin();iter_acceptDfa!=acceptDfaStates.end();iter_acceptDfa++)
			{
				tobreak=0;
				fin_eclose.clear();
				NfaStates = (*iter_acceptDfa)->GetNFAState();
				for( it=NfaStates.begin();it!=NfaStates.end();it++)
				{
					class_state *pState = *it;
					string strState = pState->GetStateID();
					if(pState->m_bAcceptingState)
					{	
						strState.erase(strState.find_first_of("{"),1);
						strState.erase(strState.find_last_of("}"),1);
					}
					fin_eclose.push_back(strState);
				}
				/*for(iter_fin_eclose=fin_eclose.begin();iter_fin_eclose!=fin_eclose.end();iter_fin_eclose++)
					cout << *iter_fin_eclose << " _______ ";
				cout << endl;*/
				for(iter_fin_eclose=fin_eclose.begin();iter_fin_eclose!=fin_eclose.end();iter_fin_eclose++)
				{
					if(*iter_fin_eclose == *iter_accept_str)
					{
						string strState = (*iter_acceptDfa)->GetStateID();
						if((*iter_acceptDfa)->m_bAcceptingState)
						{
							strState.erase(strState.find_first_of("{"),1);
							strState.erase(strState.find_last_of("}"),1);
						}
						int op = 0;
						for( op = 0; op<m_DFATable.size();op++)
						{
						 	if(int_array[op] == atoi(strState.c_str())) {break;}
						}
						//cout << atoi(strState.c_str()) << "-->> " <<op <<  endl;
						if(op == m_DFATable.size()) op = -1;
						accept_state_dfa_temp.push_back(op);
						//cout << strState << " -->>";
						acceptDfaStates.erase(iter_acceptDfa);
						tobreak=1;
						break;
					}
				}
				if(tobreak==1)	break;
			}
		}
		accept_state_dfa.push_back(accept_state_dfa_temp); //cout << "\n";
	}

	
	vector< vector<int> >::iterator iter_accept_d;
	vector<int>::iterator iter_accept_str_d;

	/*for(iter_accept_d=accept_state_dfa.begin();iter_accept_d!=accept_state_dfa.end();iter_accept_d++)
	{
		for(iter_accept_str_d=(*iter_accept_d).begin();iter_accept_str_d!=(*iter_accept_d).end();iter_accept_str_d++)
		{
			cout << *iter_accept_str_d << "--";
		}
		cout << "\n";
	}*/
}

int main()
{
	string regex_line,total_line="" ;
	int i=0;
	class_regex in;
	ifstream regex ("in");
	//ofstream dfa_transition_table ("dfa_transition_table");
	FILE *dfa_transition_table;
	dfa_transition_table=fopen("dfa_transition_table","w");
	string colon = ":";
	if(regex.is_open())
	{
		while(!regex.eof())
		{
			//get line from file
			getline(regex,regex_line);
			if(regex_line != "")
			{
				unsigned pos = regex_line.find(colon);
				token_class.push_back(regex_line.substr(0,pos));
				//cout << regex_line.substr(0,pos) << endl;
				regex_line.erase(0,pos+1);
				//cout << regex_line << endl;
				if(total_line!="")
					total_line += "|(" + regex_line + ")";
				else
					total_line = "(" + regex_line + ")";
				in.SetRegExLine(total_line);
			}
		}
		regex.close();
		in.RegExFin();

		//Save final dfa transition table to a file
		vector< vector<int> >::iterator iter_accept_d;
		vector<int>::iterator iter_accept_str_d;

		for(iter_accept_d=transition_table.begin();iter_accept_d!=transition_table.end();iter_accept_d++)
		{
			for(iter_accept_str_d=(*iter_accept_d).begin();iter_accept_str_d!=(*iter_accept_d).end();iter_accept_str_d++)
			{
				cout << *iter_accept_str_d << "\t";
				//dfa_transition_table << *iter_accept_str_d << "\t";
				fprintf(dfa_transition_table,"%d\t",*iter_accept_str_d);
			}
			cout << "\n";
			fprintf(dfa_transition_table,"\n");
			//dfa_transition_table << "\n";
		}

		//Function to traverse lexemes and 
		//output symbol table and output tokens
		symbol();
		exit(0);
	}
	else
		cout << "Could not open file\n";
}
