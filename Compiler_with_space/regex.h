#pragma once

#include <string>
#include <sstream>
#include <cassert>
#include <iostream>
#include <stdlib.h>
#include <stack>
#include <map>
#include <deque>
#include <vector>
#include <set>
#include <algorithm>
#include <list>

using namespace std;

vector< vector<string> > accept_state_nfa;
int count_accept_state_nfa = 0;

vector< vector<int> > accept_state_dfa;
int count_accept_state_dfa = 0;

vector<string> token_class;

int input_char;
vector<char> char_array;
vector< vector<int> > transition_table;

vector<int> final_state;
vector<int> int_array;

class class_regex  
{
	protected:
		//! State Class
		class class_state
		{
			protected:
				//! Transitions from this state to other 
				multimap<char, class_state*> m_Transition;

				//! State ID
				int m_nStateID;

				//! Set of NFA state from which this state is constructed
				set<class_state*> m_NFAStates;

			public:
				//! Default constructor
				class_state() : m_nStateID(-1), m_bAcceptingState(false) {};

				//! parameterized constructor
				class_state(int nID) : m_nStateID(nID), m_bAcceptingState(false) {};

				//! Constructs new state from the set of other states
				/*! This is necessary for subset construction algorithm
				  because there a new DFA state is constructed from 
				  one or more NFA states
				  */
				class_state(set<class_state*> NFAState, int nID)
				{
					m_NFAStates			= NFAState;
					m_nStateID			= nID;

					// DFA state is accepting state if it is constructed from 
					// an accepting NFA state
					m_bAcceptingState	= false;
					set<class_state*>::iterator iter;
					for(iter=NFAState.begin(); iter!=NFAState.end(); ++iter)
						if((*iter)->m_bAcceptingState)
							m_bAcceptingState = true;
				};

				//! Copy Constructor
				class_state(const class_state &other)
				{ *this = other; };

				//! Destructor
				virtual ~class_state() {};

				//! True if this state is accepting state
				bool m_bAcceptingState;

				//! Adds a transition from this state to the other
				void AddTransition(char chInput, class_state *pState)
				{
					assert(pState != NULL);
					m_Transition.insert(make_pair(chInput, pState));
				};

				//! Removes all transition that go from this state to pState
				void RemoveTransition(class_state* pState)
				{
					multimap<char, class_state*>::iterator iter;
					for(iter=m_Transition.begin(); iter!=m_Transition.end();)
					{
						class_state *toState = iter->second;
						if(toState == pState)
							m_Transition.erase(iter++);
						else ++iter;
					}
				};

				//! Returns all transitions from this state on specific input
				void GetTransition(char chInput, vector<class_state*> &States)
				{
					// clear the array first
					States.clear();

					// Iterate through all values with the key chInput
					multimap<char, class_state*>::iterator iter;
					for(iter = m_Transition.lower_bound(chInput);
							iter!= m_Transition.upper_bound(chInput);
							++iter)
					{
						class_state *pState = iter->second;
						assert(pState != NULL);
						States.push_back(pState);
					}
				};

				//! Returns the state id in form of string
				string GetStateID()
				{
					stringstream ss;
					string strRes;
					string idstr;
					//itoa(m_nStateID,idstr,10);
					ss << m_nStateID;//add number to the stream
					idstr = ss.str();
					if(m_bAcceptingState)
						strRes = "{" + idstr + "}";
					//strRes.Format("{%d}"), m_nStateID);
					else strRes = idstr;//strRes.Format("%d"), m_nStateID);
					return strRes;
				};

				/*! Returns the set of NFA states from 
				  which this DFA state was constructed
				  */
				set<class_state*>& GetNFAState()
				{ return m_NFAStates; };

				//! Returns true if this state is dead end
				/*! By dead end I mean that this state is not
				  accepting state and there are no transitions
				  leading away from this state. This function
				  is used for reducing the DFA.
				  */
				bool IsDeadEnd()
				{
					if(m_bAcceptingState)
						return false;
					if(m_Transition.empty())
						return true;

					multimap<char, class_state*>::iterator iter;
					for(iter=m_Transition.begin(); iter!=m_Transition.end(); ++iter)
					{
						class_state *toState = iter->second;
						if(toState != this)
							return false;
					}

					//TRACE("State %d is dead end.\n", m_nStateID); 

					return true;
				};	

				//! Override the assignment operator
				class_state& operator=(const class_state& other)
				{ 
					m_Transition	= other.m_Transition; 
					m_nStateID		= other.m_nStateID;
					m_NFAStates		= other.m_NFAStates;
				};

				//! Override the comparison operator
				bool operator==(const class_state& other)
				{
					if(m_NFAStates.empty())
						return(m_nStateID == other.m_nStateID);
					else return(m_NFAStates == other.m_NFAStates);
				};
		};
		
		vector< set<class_state*> > eclose;
		vector<class_state*> eclose_state;
		
		//! NFA Table
		typedef deque<class_state*> FSA_TABLE;

		/*! NFA Table is stored in a deque of class_states.
		  Each class_state object has a multimap of 
		  transitions where the key is the input
		  character and values are the references to
		  states to which it transfers.
		  */
		FSA_TABLE m_NFATable;

		//! DFA table is stores in same format as NFA table
		FSA_TABLE m_DFATable;

		//! Operand Stack
		/*! If we use the Thompson's Algorithm then we realize
		  that each operand is a NFA automata on its own!
		  */
		stack<FSA_TABLE> m_OperandStack;

		//! Operator Stack
		/*! Operators are simple characters like "*" & "|" */
		stack<char> m_OperatorStack;

		//! Keeps track of state IDs
		int m_nNextStateID;

		//! Set of input characters
		set<char> m_InputSet;

		//! Constructs basic Thompson Construction Element
		/*! Constructs basic NFA for single character and
		  pushes it onto the stack.
		  */
		void Push(char chInput);

		//! Pops an element from the operand stack
		/*! The return value is true if an element 
		  was poped successfully, otherwise it is
		  false (syntax error) 
		  */
		bool Pop(FSA_TABLE &NFATable);

		//! Checks is a specific character and operator
		bool IsOperator(char ch) { return((ch == 42) || (ch == 124) || (ch == 40) || (ch == 41) || (ch == 8)); };

		//! Returns operator presedence
		/*! Returns true if presedence of opLeft <= opRight.

		  Kleens Closure	- highest
		  Concatenation	- middle
		  Union			- lowest
		  */
		bool Presedence(char opLeft, char opRight)
		{
			if(opLeft == opRight)
				return true;

			if(opLeft == '*')
				return false;

			if(opRight == '*')
				return true;

			if(opLeft == 8)
				return false;

			if(opRight == 8)
				return true;

			if(opLeft == '|')
				return false;

			return true;
		};

		//! Checks if the specific character is input character
		bool IsInput(char ch) { return(!IsOperator(ch)); };

		//! Checks is a character left parantheses
		bool IsLeftParanthesis(char ch) { return(ch == 40); };

		//! Checks is a character right parantheses
		bool IsRightParanthesis(char ch) { return(ch == 41); };

		//! Evaluates the next operator from the operator stack
		bool evaluate();

		//! Evaluates the concatenation operator
		/*! This function pops two operands from the stack 
		  and evaluates the concatenation on them, pushing
		  the result back on the stack.
		  */
		bool concat();

		//! Evaluates the Kleen's closure - star operator
		/*! Pops one operator from the stack and evaluates
		  the star operator on it. It pushes the result
		  on the operand stack again.
		  */
		bool star();

		//! Evaluates the union operator
		/*! Pops 2 operands from the stack and evaluates
		  the union operator pushing the result on the
		  operand stack.
		  */
		bool Union();

		//! Inserts char 8 where the concatenation needs to occur
		/*! The method used to parse regular expression here is 
		  similar to method of evaluating the arithmetic expressions.
		  A difference here is that each arithmetic expression is 
		  denoted by a sign. In regular expressions the concatenation
		  is not denoted by ny sign so I will detect concatenation
		  and insert a character 8 between operands.
		  */
		string concat_expand(string strRegEx);

		//! Creates Nondeterministic Finite Automata from a Regular Expression
		bool create_nfa(string strRegEx);

		//! Calculates the Epsilon Closure 
		/*! Returns epsilon closure of all states
		  given with the parameter.
		  */
		void epsilon_closure(set<class_state*> T, set<class_state*> &Res); 

		//! Calculates all transitions on specific input char
		/*! Returns all states rechible from this set of states
		  on an input character.
		  */
		void move(char chInput, set<class_state*> T, set<class_state*> &Res);

		//! Converts NFA to DFA using the Subset Construction Algorithm
		void convert_nfa_to_dfa();

		//! Optimizes the DFA
		/*! This function scanns DFA and checks for states that are not
		  accepting states and there is no transition from that state
		  to any other state. Then after deleting this state we need to
		  go through the DFA and delete all transitions from other states
		  to this one.
		  */
		void reduce_dfa();

		//! Cleans up the memory
		void CleanUp()
		{
			// Clean up all allocated memory for NFA
			for(int i=0; i<m_NFATable.size(); ++i)
				delete m_NFATable[i];
			m_NFATable.clear();

			// Clean up all allocated memory for DFA
			for(int i=0; i<m_DFATable.size(); ++i)
				delete m_DFATable[i];
			m_DFATable.clear();

			// Reset the id tracking
			m_nNextStateID = 0;

			// Clear both stacks
			while(!m_OperandStack.empty())
				m_OperandStack.pop();
			while(!m_OperatorStack.empty())
				m_OperatorStack.pop();

			// Clean up the Input Character set
			m_InputSet.clear();
		};

	public:
		//! Default Constructor
		class_regex();

		//! Destructor
		virtual ~class_regex();

		//! Set Regular Expression
		/*! Set the string pattern to be searched for.
		  Returns true if success othewise it returns false.
		  */
		bool SetRegExLine(string strRegEx);
		void closureall();
		void RegExFin();

		string save_dfa_table()
		{
			string strDFATable;
			int count = 0;
			// First line are input characters
			set<char>::iterator iter;
			for(iter = m_InputSet.begin(); iter != m_InputSet.end(); ++iter) count++;
			
			//char_array = new char[count];
			//char_array= (char*)malloc(count*sizeof(char));
			int count_char = 0;
			char_array.clear();
			for(iter = m_InputSet.begin(); iter != m_InputSet.end(); ++iter)
			{
				char_array.push_back(*iter);
				//char_array[count_char] = *iter;
				count_char++; 
				//cout << char_array[count_char-1] << endl;			
				strDFATable += "\t\t" + string(1,*iter);
			}
			input_char = m_InputSet.size();	
			strDFATable += "\n";

			// Now go through each state and record the transitions
			int row=0, column=0;
			/*transition_table = (int**)malloc(m_DFATable.size()*sizeof(int*));
			for(int i=0; i<m_DFATable.size(); ++i) transition_table[i] = (int*)malloc(count*sizeof(int));*/
			//int_array = (int*)malloc(m_DFATable.size()*sizeof(int));
			int count_int = 0;
			int_array.clear();
			for(int i=0; i<m_DFATable.size(); ++i)
			{
				class_state *pState1 = m_DFATable[i];
				string temp = pState1->GetStateID();
				if(temp[0] == '{')
				{
					temp.erase(temp.find_first_of("{"),1);
					temp.erase(temp.find_last_of("}"),1);
				}
				int_array.push_back(atoi(temp.c_str()));
				//int_array[count_int] = atoi(temp.c_str());
				//cout << atoi(temp.c_str()) << "-->  ";
				count_int++;
			}
			/*vector<int>::iterator iter_int_arr;
			for(iter_int_arr=int_array.begin();iter_int_arr!=int_array.end();iter_int_arr++)
			{
				cout << *iter_int_arr << " -->> ";
			}*/
			transition_table.clear();
			for(int i=0; i<m_DFATable.size(); ++i)
			{
				class_state *pState = m_DFATable[i];

				// Save the state id
				strDFATable += pState->GetStateID();

				// now write all transitions for each character
				vector<class_state*> State;
				vector<int> trans_tab;
				column = 0;
				for(iter = m_InputSet.begin(); iter != m_InputSet.end(); ++iter)
				{
					pState->GetTransition(*iter, State);
					string strState;
					if(State.size()>0)
					{
						strState = State[0]->GetStateID();
						for(int i=1; i<State.size(); ++i)
							strState += "," + State[i]->GetStateID();
					}
					strDFATable += "\t\t" + strState;
					if(strState=="") strState="-1";
					if(strState[0] == '{')
					{
						strState.erase(strState.find_first_of("{"),1);
						strState.erase(strState.find_last_of("}"),1);
					}
					int temp = atoi(strState.c_str());
					int op = 0;
					for( op = 0; op<count_int;op++)
					{	
					 if(int_array[op] == temp){ break;}
					}
					
					if(op == count_int) op = -1;
					trans_tab.push_back(op);
					//transition_table[row][column] = op;
					column++;		
				}
				transition_table.push_back(trans_tab);
				row++;
				strDFATable += "\n";
			}
			//cout  << endl<<endl; 
			/*for(int i=0;i<row;i++)
			{
				for(int j=0;j<column;j++) cout << transition_table[i][j] << "\t";
				cout << endl;
			}*/
			int count_accept = 0;
			for(int i=0; i<m_DFATable.size(); ++i)
			{
				class_state *pState = m_DFATable[i];
				if(pState->m_bAcceptingState)
					count_accept++;
			}
			int fin = 0;
			//final_state = (int*)malloc(count*sizeof(int));
			//cout << endl << endl;
			char test_array[count];
			for(int i=0; i<m_DFATable.size(); ++i)
			{
				class_state *pState = m_DFATable[i];
				if(pState->m_bAcceptingState)
				{
					string strStateID = pState->GetStateID();
                                        strStateID.erase(strStateID.find_first_of("{"),1);strStateID.erase(strStateID.find_last_of("}"),1);
					int temp = atoi(strStateID.c_str());					
					int op = 0;
					for(op = 0; op<count_int;op++)
					{	
					 if(temp == int_array[op]) break;
					}
					final_state.push_back(op);
					//final_state[fin] = op;
					//cout<< final_state[fin]<< "\t";			
					fin++;
				}
			}
			//cout << endl;
			
			return strDFATable;
		};
};
