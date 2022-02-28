#include <string>
#include<string.h>
#include<vector>
#include<algorithm>
#include <cctype>
#include<stack>

using namespace std;


float variabile[26];
#define NR_OPERATORI 40
string operatori[] = {"+","/","-","*","%", "(", ")",",", "=", "+=","-=","*=","/=","%=", "&&", "||", "<","<=",">",">=", "==", "!=", "<<", ">>", "&","^","|", ">>=", "<<=", "&=", "^=", "|=","~","!","+++","---","++P","P++","--P","P--"};
int eroare = 0;

bool esteVariabila(char ch)
{
    return isalpha(ch) && islower(ch);
}

float extrage_numar(string s,int& i)
{
    float sol = 0;
    float fractional = 0;
    float p = 1;

    while(i < s.size() && isdigit(s[i]))
    {
        sol = sol * 10 + (s[i] - '0');
        i++;
    }
    if(i < s.size() &&  s[i] == '.')
    {
        i++;
        if(i == s.size() || !isdigit(s[i]))
            eroare = 1;
    }

    while(i < s.size() && isdigit(s[i]))
    {
        p = p * 10;
        fractional = fractional * 10 + (s[i] - '0');
        i++;
    }

    return sol + (float(fractional) / float(p));
}

bool prefixOperator(char ch1, char ch2, char ch3)
{
    if(ch1 == '+' && ch2 == '+' && (isalpha(ch3) || ch3 == ch2 || isdigit(ch3)) )
        return true;
    else if(ch1 == '-' && ch2 == '-' && (isalpha(ch3) || ch3 == ch2 ||isdigit(ch3)))
        return true;
    return false;
}

bool postfixOperator(char ch1,char ch2,char ch3)
{
    if(isalpha(ch1) && ch2 == '+' && ch3 == '+')
        return true;
    else if(isalpha(ch1) && ch2 == '-' && ch3 == '-')
        return true;
    return false;
}

bool operatorEgalitate(char ch) // toti operatorii de tipul op=
{
    return (ch == '<' || ch == '>' || ch == '=' || ch == '!' || ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%' || ch == '&' || ch == '^' || ch == '|' || ch == '~');
}

bool operatoriAtribuire(string ch)
{
    return ch == "+=" || ch == "-=" || ch ==  "*=" || ch == "/=" || ch == "%=" || ch == "<<=" || ch == ">>=" || ch == "&=" || ch == "^=" || ch == "|=";
}

float convertesteNumar(string numar)
{
    int negativ = 0;  //0 = + , 1 = -
    int i = 0;

    if(!isdigit(numar[0]))
    {
        if(numar[0] == '-')
            negativ = 1;
        i = 1;
    }

    float nr = extrage_numar(numar,i);
    if(negativ)
        nr = nr * -1;

    return nr;
}

bool esteOperator(string s)
{

    for(int i = 0; i < NR_OPERATORI; i++)
    {
        if(s == operatori[i])
            return true;
    }
    return false;
}

float applyOperation(float a,float b,string op) // pentru calcul algebric
{
    if(op == "+")
        return a + b;
    else if(op == "-")
        return b - a;
    else if(op == "*")
        return a * b;
    else if(op == "/")
    {
        if(a == 0)
        {
            eroare = 16;
            return 0;
        }
        else
            return float(b) / float(a);
    }
    else if(op == "&&")
    {
        return a && b;
    }
    else if(op == "||")
    {
        return a || b;
    }
    else if(op == "<=")
    {
        return b <= a;
    }
    else if(op == "<")
    {
        return b < a;
    }
    else if(op == ">")
    {
        return b > a;
    }
    else if(op == ">=")
    {
        return b >= a;
    }
    else if(op == "==")
    {
        return a == b;
    }
    else if(op == "!=")
    {
        return a != b;
    }
    else if(op == "%" || op == "^" || op == "|" || op == "&" || op == "<<" || op == ">>")
    {
        int a1 = a;
        int b1 = b;
        if(a1 == a && b1 == b)
        {
            if(op == "%")
            {
                if(a1 == 0)
                {
                    eroare = 16;
                    return 0;
                }
                else
                    return b1 % a1;
            }
            if(op == "^")
                return b1 ^ a1;
            if(op == "&")
                return b1 & a1;
            if(op == "|")
                return b1 | a1;
            if(op == "<<")
                return b1 << a1;
            if(op == ">>")
                return b1 >> a1;
        }
        else
        {
            eroare = 17;
            return 0;
        }
    }

    eroare = 18;
    return 0;
}

void atribuire(stack<string>& valori,char ch)
{
    string nr1,nr2;
    float a,b;
    char variabila, operatie;

    if(valori.empty())
        eroare = 15;
    else
    {
        nr1 = valori.top();
        valori.pop();
    }

    if(valori.empty())
        eroare = 15;
    else
    {
        nr2 = valori.top();
        valori.pop();
    }

    if(nr2.size() == 1 && isalpha(nr2[0]))
        variabila = nr2[0];
    else
        eroare = 15;

    if(!eroare)
    {
        a = variabile[variabila - 'a'];

        if(nr1.size() == 1 && isalpha(nr1[0]))
            b = variabile[nr1[0]-'a'];
        else
            b = convertesteNumar(nr1);

        if(ch == '~')
        {
            variabile[variabila - 'a'] = b;
        }
        else if(ch == '+')
        {
            variabile[variabila - 'a'] = applyOperation(b,a,"+");
        }
        else if(ch == '-')
        {
            variabile[variabila - 'a'] = applyOperation(b,a,"-");
        }
        else if(ch == '*')
        {
            variabile[variabila - 'a'] = applyOperation(b,a,"*");
        }
        else if(ch == '/')
        {
            variabile[variabila - 'a'] = applyOperation(b,a,"/");
        }
        else if(ch == '%')
        {
            variabile[variabila - 'a'] = applyOperation(b,a,"%");
        }
        else if(ch == '>')
        {
            variabile[variabila - 'a'] = applyOperation(b,a,">>");
        }
        else if(ch == '<')
        {
            variabile[variabila - 'a'] = applyOperation(b,a,"<<");
        }
        else if(ch == '&')
        {
            variabile[variabila - 'a'] = applyOperation(b,a,"&");
        }
        else if(ch == '|')
        {
            variabile[variabila - 'a'] = applyOperation(b,a,"|");
        }
        else if(ch == '^')
        {
            variabile[variabila - 'a'] = applyOperation(b,a,"^");
        }
        valori.push({variabila});
    }

    if(eroare)
        valori.push("0");
}

void operatieStiva(stack<string>& operatori, stack<string>& valori)
{

    string op = operatori.top();
    string nr1,nr2;
    float a,b;

    if(op == "+" || op == "-" || op == "*" || op == "/" || op == "%" || op == "&&" || op == "||" || op == "<=" || op == "<" || op == ">" || op == ">=" || op == "!=" || op == "==" || op == "&" || op == "|" || op == "^" || op == ">>" || op == "<<")
    {
        if(valori.empty())
            eroare = 15;
        else
        {
            nr1 = valori.top();
            valori.pop();
        }

        if(valori.empty())
            eroare = 15;
        else
        {
            nr2 = valori.top();
            valori.pop();
        }

        if(nr1.size() == 1 && esteVariabila(nr1[0]))
            a = variabile[nr1[0]-'a'];
        else
            a = convertesteNumar(nr1);

        if(nr2.size() == 1 && isalpha(nr2[0]))
            b = variabile[nr2[0]-'a'];
        else
            b = convertesteNumar(nr2);

        valori.push(to_string(applyOperation(a,b,op)) );

    }
    else if(op == "!" || op == "+++" || op == "---" || op == "~")
    {

        if(valori.empty())
            eroare = 15;
        else
        {
            nr1 = valori.top();
            valori.pop();
        }

        if(nr1.size() == 1 && isalpha(nr1[0]))
            a = variabile[nr1[0]-'a'];
        else if(nr1.size() == 3 && isalpha(nr1[0]))
        {
            if(nr1[1] == '+')
                a = variabile[nr1[0]-'a']++;
            else
                a = variabile[nr1[0]-'a']--;
        }
        else if(nr1.size() == 3 && isalpha(nr1[2]))
        {
            if(nr1[1] == '+')
                a = ++variabile[nr1[2]-'a'];
            else
                a = --variabile[nr1[2]-'a'];
        }
        else
            a = convertesteNumar(nr1);

        if(op == "!")
            valori.push(to_string(!a));
        else if(op == "~")
        {
            int intreg = a;
            if(intreg != a)
                eroare = 17;
            else
                valori.push(to_string(~intreg));

        }
        else if(op == "+++")
            valori.push(to_string(a));
        else if(op == "---")
            valori.push(to_string(-a));


    }
    else if(op == "=")
    {
        atribuire(valori,'~');
    }
    else if(operatoriAtribuire(op))
    {
        atribuire(valori,op[0]);
    }
    else if(op == "++P" || op == "--P")
    {
        if(valori.empty())
            eroare = 15;
        else
        {
            nr1 = valori.top();
            valori.pop();
        }

        if(esteVariabila(nr1[0]) && nr1.size() == 1)
        {

            if(op[0] == '+')
                a = ++variabile[nr1[0]-'a'];
            else
                a = --variabile[nr1[0]-'a'];
            valori.push({nr1[0]});
        }
        else
        {
            eroare = 15;
        }
    }
    else if(op == "P++" || op == "P--")
    {
        if(valori.empty())
            eroare = 15;
        else
        {
            nr1 = valori.top();
            valori.pop();
        }

        if(esteVariabila(nr1[0]) && nr1.size() == 1)
        {
            valori.push(to_string(variabile[nr1[0]-'a']));
            if(op[1] == '+')
                a = variabile[nr1[0]-'a']++;
            else
                a = variabile[nr1[0]-'a']--;
        }

    }


}

int precedence(string a)
{
    if(a == "P++" || a == "P--")
        return 2;
    else if(a == "!" || a == "+++" || a == "---" || a == "~" || a == "++P" || a == "--P")
        return 3;
    else if(a == "*" || a == "/" || a == "%")
        return 5;
    else if(a == "+" || a == "-")
        return 6;
    else if(a == ">>" || a == "<<")
        return 7;
    else if(a == "<" || a == "<=" || a == ">" || a == ">=")
        return 9;
    else if(a == "==" || a == "!=")
        return 10;
    else if(a == "&")
        return 11;
    else if(a == "^")
        return 12;
    else if(a == "|")
        return 13;
    else if(a == "&&")
        return 14;
    else if(a == "||")
        return 15;
    else if(a == "=" || a == "+=" || a == "-=" || a == "/=" || a == "*=" || a == "%=" || a == "<<=" || a == ">>=" || a == "&=" || a == "^=" || a == "|=")
        return 16;
    else if(a == ",")
        return 17;

    return 18;
}

bool rtl(string op)
{
    return (op == "+++" || op == "---" || op == "!" || op == "~" || op == "+=" || op == "-=" || op == "=" || op == "*=" || op == "/=" || op == "%=" || op == "<<=" || op == ">>=" || op == "&=" || op == "|=" || op == "^=" || op == "++P" || op == "--P");
}

float executa(vector<string> tokens)
{

    stack<string> operatori;
    stack<string> valori;

    for(int i = 0; i<tokens.size() && !eroare; i++)
    {
        string cur = tokens[i];


        if(!esteOperator(cur))
        {
            valori.push(cur);
        }
        else if(cur == "(")
        {
            operatori.push(cur);

        }
        else if(cur == ")")
        {
            while(!operatori.empty() && operatori.top() != "(")
            {
                operatieStiva(operatori,valori);
                operatori.pop();
            }
            if(operatori.empty())
                eroare = 8;
            else
                operatori.pop();
        }
        else
        {
            if(cur == ",")
            {
                while(!operatori.empty() && operatori.top() != "(")
                {
                    operatieStiva(operatori,valori);
                    operatori.pop();
                }
                if(valori.empty())
                    eroare = 21;
                else
                    valori.pop();
            }
            else if(!operatori.empty() && rtl(operatori.top()))
            {
                while(!operatori.empty() && operatori.top() != "(" && precedence(cur) > precedence(operatori.top()))
                {
                    operatieStiva(operatori,valori);
                    operatori.pop();
                }
                operatori.push(cur);
            }
            else
            {
                while(!operatori.empty() && operatori.top() != "(" && precedence(cur) >= precedence(operatori.top()))
                {
                    operatieStiva(operatori,valori);
                    operatori.pop();
                }
                operatori.push(cur);
            }

        }
    }


    while(!operatori.empty() && !eroare)
    {
        operatieStiva(operatori,valori);
        operatori.pop();
    }
    if(valori.empty()) // cazul ()
        return 0;
    else if(isalpha(valori.top()[0]))
        return variabile[valori.top()[0]-'a'];
    else
        return convertesteNumar(valori.top());

    return 0;
}

void verificaTokens(string s,vector<string>& tokens)
{
    for(int i = 1; i<s.size() && !eroare; i++) // doua caractere egale consecutive
        if(esteVariabila(s[i]) && esteVariabila(s[i-1]))
            eroare = 13;

    for(int i=0; i<tokens.size() && !eroare; i++)
    {
        string token = tokens[i];

        if(i == 0 && esteOperator(token) && token != "(" && token != "!" && token != "~"&& token != "+++" && token != "---" && token != "++P")
            eroare = 22;

        if(i != 0 && tokens[i] == ")" && tokens[i-1] == "(")
            eroare = 23;

        if(i == tokens.size() - 1 && esteOperator(token) && token != ")" && tokens[i] != "P++" && tokens[i] != "P--" && tokens[i] != "++P" && tokens[i] != "--P") // operator la finalul expresiei
            eroare = 12;
        //doi operatori unul langa altul
        if(i >= 1 && tokens[i] != "+++" && tokens[i] != "---" && tokens[i] != "!" && tokens[i] != "~" &&  tokens[i] != "(" && tokens[i] != ")" && esteOperator(tokens[i]) && esteOperator(tokens[i-1]) && tokens[i-1] != "(" && tokens[i-1] != ")"  && tokens[i-1] != "!" && tokens[i-1] != "+++" && tokens[i-1] != "---" && tokens[i-1] != "!" && tokens[i-1] != "~" && tokens[i-1] != "++P" && tokens[i-1] != "--P" && tokens[i-1] != "P++" && tokens[i-1] != "--P")
            eroare = 14;
    }

}

void obtineTokens(string s,vector<string>& tokens)
{
    for(int i = 0; i < s.size() && !eroare; i++)
    {
        if(isdigit(s[i])) // daca avem un numar
        {
            int j = i;
            float nr = extrage_numar(s,j); // extragem intreg numarul
            i = j - 1;
            tokens.push_back(to_string(nr));
        }
        else if(s[i] == 'P') // Plusul unar
            tokens.push_back("+++");
        else if(s[i] == 'M') // Minusul unar
            tokens.push_back("---");
        else if(esteVariabila(s[i]))
        {
            if(i < s.size()-2 && s[i+2] == '#')
            {
                if(s[i+1] == '+') // daca i o expresie de forma a++ a--
                {
                    tokens.push_back({s[i]});
                    tokens.push_back("P++");

                }
                else
                {
                    tokens.push_back({s[i]});
                    tokens.push_back("P--");
                }
                i+=2;
            }
            else
                tokens.push_back({s[i]}); // daca i o variabila
        }
        else if(operatorEgalitate(s[i]))
        {
            if(i < s.size()-1)
            {
                if(s[i+1] == 'E')
                {
                    tokens.push_back({s[i],'='}); // a += b, a -= b, ...
                    i++;
                }
                else if(s[i] == '&' && s[i+1] == 'A')
                {
                    tokens.push_back({s[i],s[i]}); // a && b
                    i++;
                }
                else if(s[i] == '|' && s[i+1] == 'O')
                {
                    tokens.push_back({s[i],s[i]}); // a || b
                    i++;
                }
                else if(s[i] == '<' && s[i+1] == 'L')
                {
                    tokens.push_back({s[i],s[i]}); // a << b
                    i++;
                }
                else if(s[i] == '>' && s[i+1] == 'G')
                {
                    tokens.push_back({s[i],s[i]}); // a >> b
                    i++;
                }
                else if( (s[i] == '<' || s[i] == '>') && s[i+1] == 'C')
                {
                    tokens.push_back({s[i],s[i],'='}); // a >>= b, a <<= b
                    i+=2;
                }
                else
                    tokens.push_back({s[i]}); // operatii elementare: + - * / ^ % & | < > =
            }
            else
            {
                eroare = 12; // un operator la final
            }
        }
        else if(s[i] == '@')
        {
            if(s[i+1] == '+') // ++a, --a
            {
                    if(esteVariabila(s[i+2]))
                    {
                        tokens.push_back({s[i+2]});
                        i++;
                    }
                    tokens.push_back("++P");
            }
            else
            {
                    if(esteVariabila(s[i+2]))
                    {
                        tokens.push_back({s[i+2]});
                        i++;
                    }
                    tokens.push_back("--P");
            }
            i++;
        }
        else if(s[i] == ',' || s[i] == '(' || s[i] == ')') // simboluri auxiliare
            tokens.push_back({s[i]});
    }

}

bool caracterPermis(char ch)
{
    char permise[] = "0123456789-+,/><=abcdefghijklmnopqrstuwqyz!%^&*()~ .|";
    return strchr(permise,ch) != NULL;
}

bool verificaPuncte(string s)
{
    if(s[0] == '.' || s[s.size()-1] == '.')
        eroare = 9;

    for(int i=1; i<s.size(); i++)
    {
        if(s[i] == s[i+1] && s[i] == '.')
            eroare = 10;
        if(i <= s.size()-2 && s[i] == '.' && (!isdigit(s[i+1]) || !isdigit(s[i-1])))
            eroare = 11;
    }
}

void verificaParanteze(string s)
{
    stack<char> st;
    char pred = -1;

    for(int i=0; i<s.size(); i++)
    {
        if(s[i] == '(')
        {
            if(pred == ')' && st.empty())
            {
                eroare = 8;
            }
            else
            {
                st.push('(');
                pred = '(';
            }
        }
        else if(s[i] == ')')
        {
            if(st.empty())
            {
                eroare = 8;
            }
            else
                st.pop();
        }
        else if(s[i] != ' ')
            pred = s[i];

    }

    if(!st.empty())
    {
        eroare = 8;
    }
}

void marcheazaSirFaraSpatii(string& s)
{
    for(int i=0; i<s.size() && !eroare; i++)
    {
        if(i == 0 && i < s.size()-1)
        {
            if(s[i] == '+') // +2
            {
                s[i] = 'P'; // plus
            }
            else if(s[i] == '-') // -2
            {
                s[i] = 'M'; // minus
            }
        }
        else if(i < s.size())
        {
            if( (s[i] == '+' || s[i] == '-') && (s[i-1] == 'P' || s[i-1] == 'M' || s[i-1] == 'E' || s[i-1] == 'L' || s[i-1] == 'G' || s[i-1] == 'A' || s[i-1] == 'O' || esteOperator({s[i-1]})) )
            {
                if(s[i] == '+')
                    s[i] = 'P';
                else if(s[i] == '-')
                    s[i] = 'M';
            }
        }
    }
}

void marcheazaSir(string& s)
{
    // marcheaza operatorii expresiei pentru extragerea tokenurilor
    for(int i=0; i < s.size() && !eroare; i++)
    {
        if(i < s.size()-2)
        {
            // expresia ++a, --a => @+a @-a
            if(prefixOperator(s[i],s[i+1],s[i+2]))
            {
                s[i] = '@';
                if(!esteVariabila(s[i+2]))
                    i++;
                else
                    i+=2;
                continue;
            }

            //expresia a++, a-- => a+# a-#
            else if(postfixOperator(s[i],s[i+1],s[i+2]))
            {
                s[i+2] = '#';
                i+=2;
                continue;
            }
            //expresia a<<=b, a>>=b => a<CEb a>CEb
            else if(s[i] == s[i+1] && (s[i] == '<' || s[i] == '>') && s[i+2] == '=')
            {
                s[i+1] = 'C';
                s[i+2] = 'E';
                i+=2;
                continue;
            }
        }

        if(i < s.size()-1)
        {
            // a<<b => a<Lb
            if(s[i] == '<' && s[i+1] == '<')
            {
                s[i+1] = 'L';
                i++;
            }
            // a>>b => a>Gb
            else if(s[i] == '>' && s[i+1] == '>')
            {
                s[i+1] = 'G';
                i++;
            }
            // a+=5, b-=v, c*=2 ... => a+E5, b-Eb, c*E2 ...
            else if(operatorEgalitate(s[i]) && s[i+1] == '=')
            {
                s[i+1] = 'E'; // egalitate
                i++;
            }
            // a && b => a &A b
            else if(s[i] == '&' && s[i+1] == '&')
            {
                s[i+1] = 'A'; // si
                i++;
            }
            // a || b => a |O b
            else if(s[i] == '|' && s[i+1] == '|')
            {
                s[i+1] = 'O'; // sau
                i++;
            }
        }
    }
}

void testareInitiala(string s)
{
    // o prima verificare a expresiei (alfabeul este corect si nu se afla 3 caractere asemanatoare unul dupa altul (cu unele exceptii)
    for(int i=0; i < s.size() && !eroare; i++)
    {
        if(s[i] == '!' || s[i] == '~') // operatorii unari ! si ~ pot sa apara de cate ori se poate in mod consecutiv
            continue;
        else if(s[i] == '+' || s[i] == '-') // operatorii + si - pot sa apara o singura data, sau de un numar par de ori consecutiv ++++a ii corect, dar +++a nu-i corect
        {
            int nrOperatoriUnari = 0;
            int j = i;
            while(j < s.size() && s[j] == s[i]) // numara de cate ori apare consecutiv operatorul + sau -
            {
                nrOperatoriUnari++;
                j++;
            }

            if(nrOperatoriUnari % 2 == 1 && nrOperatoriUnari != 1) // daca apar de un numar impar de ori
                eroare = 5;
            else if(nrOperatoriUnari % 2 == 0) // daca apare de un numar par de ori, atunci trebuie sa testam daca are o variabila
            {
                int okl = 1;
                int okr = 1;

                if(i == 0 || !esteVariabila(s[i-1])) // la stanga operatorului ++/--
                    okl = 0;
                else if(nrOperatoriUnari > 2)
                    eroare = 20;

                if(j == s.size() || !esteVariabila(s[j])) // la dreapta operatorului ++/--
                    okr = 0;
                if(!okl && !okr) // daca nu apare o variabila strict inaintea sau strict dupa operatorul ++/--
                    eroare = 19;
            }
            i = j - 1;
        }
        else if(i < s.size()-2 && s[i] == s[i+1] && s[i+1] == s[i+2] && s[i] != '(' && s[i] != ')' && s[i] != ' ') // 3 simboluri consecutive de acelasi fel nu sunt permise
            eroare = 5;
        else if(!caracterPermis(s[i])) // expresia trebuie sa se afle peste alfabetul corect, adica sa nu apara caractere neacceptate
            eroare = 6;

    }
}

float evalueazaExpresie(char sir[])
{
    eroare = 0; // detecteaza orice fel de eroare
    string s(sir); // stocam expresia initiala
    vector<string> tokens; // o expresie este impartita in mai multi tokeni

    testareInitiala(s);

    marcheazaSir(s);

    s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end()); // eliminam spatiile dintre operatori si cifre

    marcheazaSirFaraSpatii(s);

    verificaParanteze(s); // verifica daca parantizarea este corecta

    verificaPuncte(s);

    obtineTokens(s,tokens);

    verificaTokens(s,tokens);

    if(!eroare)
    {
        float sol = executa(tokens);
        return sol;
    }
    else
        return -1;

    return 0;


}
