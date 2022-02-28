
#define LUNG_MAX 50
#define ELEM_MAX 30

int schemaGresita = 0;
int startInceput = 0;
int instructiunePrecedenta = 0;
stack<int> inceputBucla;


struct nod {
    char eticheta[LUNG_MAX];
    char info[LUNG_MAX];
    int len=0;
    int nrFii;
    int componenta;

    nod* fiu[ELEM_MAX];
};


void citeste_eticheta(FILE* fin, nod* obiect)
{
    char c = fgetc(fin);
    int i = 0,nrParanteze = 0;

    while(c != -1 && (c != '>' && c != '<' || nrParanteze) && !schemaGresita) // o eticheta corecta ii de forma < nume/atribute >
    {
        if(c == '(') // daca apar paranteze ne asiguram ca acestea se inchid corect
            nrParanteze++;
        else if(c == ')')
            nrParanteze--;
        if(nrParanteze < 0)
            schemaGresita = 1;

        obiect->eticheta[i++] = c; // adaugam caracterul in eticheta nodului curent
        c = fgetc(fin);
    }

    if(c != '>' || nrParanteze)
        schemaGresita = 1; // daca nu se termina corect eticheta
    obiect->eticheta[i] = '\0';
}

bool verificaEticheta(nod* obiect, char cautat[]) // verifica daca eticheta unui nod ii un cuvant indicat
{
    int i = 0, len1 = strlen(obiect->eticheta), len2 = strlen(cautat);

    while(i < len1 && i < len2 && obiect->eticheta[i] == cautat[i])
        i++;
    if(i < len2 || i < len1 && obiect->eticheta[i] != ' ')
        return false;

    return true;
}

bool verificaEticheta(char mes[], char cautat[])
{
    int i = 0,len1 = strlen(mes),len2 = strlen(cautat);

    while(i < len1 && i < len2 && mes[i] == cautat[i])
        i++;
    if(i < len2 || i < len1 && mes[i] != ' ')
        return false;
    return true;
}

bool verificaAtribute(nod* schema, char tip[])
{
    if(strcmp(tip,"conditie") == 0)
    {
        char atributCautat[] = "cond=";
        int i = 0, len = strlen(schema->eticheta);

        while(i < len && schema->eticheta[i] != ' ')
            i++;

        if(i >= len) // daca nu are atributul cond=
            return false;

        while(i < len && schema->eticheta[i] == ' ')
            i++;

        if(i >= len) // daca nu are atributul cond=
            return false;

        for(int j=0;j < strlen(atributCautat) && i < len;j++, i++)
            if(schema->eticheta[i] != atributCautat[j])
                return false;

        if(i >= len)
            return false;

        return true;
    }
    else if(strcmp(tip,"pentru") == 0)
    {
        char atributCautat[][6] = {"i=","cond=","inc="};
        int i = 0, len = strlen(schema->eticheta);

        while(i < len && schema->eticheta[i] != ' ')
            i++;

        if(i >= len) // daca nu are atributul i=
            return false;

        while(i < len && schema->eticheta[i] == ' ')
            i++;

        if(i >= len) // daca nu are atributul i=
            return false;

        for(int j=0;j < strlen(atributCautat[0]) && i < len;j++, i++)
            if(schema->eticheta[i] != atributCautat[0][j])
                return false;

        if(i >= len) // daca nu are atributul cond=
            return false;

        int d = 0;
        while(i < len && (schema->eticheta[i] != ' ' || d != 0) && d >= 0 )
        {
            if(schema->eticheta[i] == '(')
                d++;
            else if(schema->eticheta[i] == ')')
                d--;
            i++;
        }

        if(i >= len) // daca nu are atributul i=
            return false;

        d = 0;

        if(i >= len) // daca nu are atributul cond=
            return false;

        while(i < len && schema->eticheta[i] == ' ')
            i++;

        for(int j=0;j < strlen(atributCautat[1]) && i < len;j++, i++)
            if(schema->eticheta[i] != atributCautat[1][j])
                return false;

        if(i >= len) // daca nu are atributul inc=
            return false;

        while(i < len && (schema->eticheta[i] != ' ' || d != 0) && d >= 0 )
        {
            if(schema->eticheta[i] == '(')
                d++;
            else if(schema->eticheta[i] == ')')
                d--;
            i++;
        }

        if(i >= len) // daca nu are atributul cond=
            return false;

        while(i < len && schema->eticheta[i] == ' ')
            i++;

        if(i >= len) // daca nu are atributul inc=
            return false;

        for(int j=0;j < strlen(atributCautat[2]) && i < len;j++, i++)
            if(schema->eticheta[i] != atributCautat[2][j])
                return false;

        if(i >= len)
            return false;

        return true;
    }

    return false;
}

void testeazaEticheta(nod* schema)
{
    if(verificaEticheta(schema,"start")) // o eticheta start nu poate sa aibe atribute si este de forma <start>, in plus exista doar un start in tot codul
        if(strlen(schema->eticheta) != 5)
            schemaGresita = 1;
        else
            startInceput++;

    else if(verificaEticheta(schema,"calcul") && strlen(schema->eticheta) != 6) // o eticheta calcul nu poate sa aibe atribute si este de forma <calcul>
        schemaGresita = 1;

    else if(verificaEticheta(schema,"citeste") && strlen(schema->eticheta) != 7) // o eticheta citeste nu poate sa aibe atribute si este de forma <citeste>
        schemaGresita = 1;

    else if(verificaEticheta(schema,"daca") && !verificaAtribute(schema,"conditie")) //o eticheta daca are atributul cond si este de forma <daca cond=expr>
        schemaGresita = 1;

    else if(verificaEticheta(schema,"altfel") && (strlen(schema->eticheta) != 6 || instructiunePrecedenta != 4)) // o eticheta altfel nu poate sa aibe atribute si este de forma <altfel> si urmeaza dupa un <daca> sau <altfelDaca>
        schemaGresita = 1;

    else if(verificaEticheta(schema,"altfelDaca") && (!verificaAtribute(schema,"conditie") || instructiunePrecedenta != 4) ) // o eticheta altfelDaca are atributul cond si este de forma <altfelDaca cond=expr> si urmeaza dupa un daca sau un alt altfelDaca
        schemaGresita = 1;

    else if(verificaEticheta(schema,"catTimp") && !verificaAtribute(schema,"conditie"))
        schemaGresita = 1;

    else if(verificaEticheta(schema,"pentru") && !verificaAtribute(schema,"pentru"))
        schemaGresita = 1;

    else if(verificaEticheta(schema,"doWhile") && !verificaAtribute(schema,"conditie"))
        schemaGresita = 1;

    if(startInceput > 1) // daca avem mai mult de o eticheta start
        schemaGresita = 1;
}

void testeazaEtichetaSlash(char etichetaStop[], stack<nod*>& s)
{

    if(verificaEticheta(etichetaStop,"start") && strlen(etichetaStop) == 5)
    {
        if(!verificaEticheta(s.top(),"start"))
            schemaGresita = 1;
        instructiunePrecedenta = 0;
    }
    else if(verificaEticheta(etichetaStop,"calcul") && strlen(etichetaStop) == 6)
    {
        if(!verificaEticheta(s.top(),"calcul"))
                schemaGresita = 1;
        instructiunePrecedenta = 5;
    }
    else if(verificaEticheta(etichetaStop,"citeste") && strlen(etichetaStop) == 7)
    {
        if(!verificaEticheta(s.top(),"citeste"))
            schemaGresita = 1;
        instructiunePrecedenta = 2;
    }
    else if(verificaEticheta(etichetaStop,"scrie") && strlen(etichetaStop) == 5)
    {
        if(!verificaEticheta(s.top(),"scrie"))
            schemaGresita = 1;
        instructiunePrecedenta = 3;
    }
    else if(verificaEticheta(etichetaStop,"daca") && strlen(etichetaStop) == 4)
    {
        if(!verificaEticheta(s.top(),"daca"))
            schemaGresita = 1;
        instructiunePrecedenta = 4;
    }
    else if(verificaEticheta(etichetaStop,"altfelDaca") && strlen(etichetaStop) == 10)
    {
        if(!verificaEticheta(s.top(),"altfelDaca"))
            schemaGresita = 1;
        instructiunePrecedenta = 4;
    }
    else if(verificaEticheta(etichetaStop,"altfel") && strlen(etichetaStop) == 6)
    {
        if(!verificaEticheta(s.top(),"altfel"))
            schemaGresita = 1;
        instructiunePrecedenta = 6;
    }
    else if(verificaEticheta(etichetaStop,"catTimp") && strlen(etichetaStop) == 7)
    {
        if(!verificaEticheta(s.top(),"catTimp"))
            schemaGresita = 1;
        instructiunePrecedenta = 7;
    }
    else if(verificaEticheta(etichetaStop,"pentru") && strlen(etichetaStop) == 6)
    {
        if(!verificaEticheta(s.top(),"pentru"))
            schemaGresita = 1;
        instructiunePrecedenta = 8;
    }
    else if(verificaEticheta(etichetaStop,"doWhile") && strlen(etichetaStop) == 7)
    {
        if(!verificaEticheta(s.top(),"doWhile"))
            schemaGresita = 1;
        instructiunePrecedenta = 8;
    }
    else
    {
        schemaGresita = 1;
    }
}

nod* generare(char numeFisier[LUNG_MAX])
{
    FILE* fin = fopen(numeFisier,"r");

    nod* obiect = new nod;
    strcpy(obiect->eticheta,"UNIVERSAL");
    obiect->nrFii = 0;

    stack<nod*> s;
    s.push(obiect);

    char c = fgetc(fin);
    while(c == ' ' || c == '\n' || c == '\t') // ignoram toate caracterele auxiliare de la inceput
        c = fgetc(fin);

    fseek(fin,-1,SEEK_CUR);

    while(!schemaGresita && !feof(fin))
    {
        c = fgetc(fin);
        if(c == -1 ||  c == '\n' || c == '\t')
            continue;

        if(c == '<')
        {
            char c = fgetc(fin);
            if(c != '/')
            {
                fseek(fin,-1,SEEK_CUR);

                nod* nou = new nod;
                nou->nrFii = 0;

                citeste_eticheta(fin,nou);
                testeazaEticheta(nou);

                nod* varf = s.top();
                if(verificaEticheta(varf, "calcul") || verificaEticheta(varf, "citeste") || verificaEticheta(varf, "scrie")) // blocurile calcul citeste si scrie nu pot sa aibe copii
                    schemaGresita = 1;
                else
                {
                    varf->fiu[varf->nrFii++] = nou;
                    s.push(nou);
                }
            }
            else
            {
                char etichetaStop[LUNG_MAX];
                int i = 0;

                c = fgetc(fin);
                while(c != -1 && c != '>')
                {
                    etichetaStop[i++] = c;
                    c = fgetc(fin);
                }
                etichetaStop[i] = '\0';

                if(s.size() == 1) // daca avem o inchidere a unei etichete neimperecheate
                {
                    schemaGresita = 1;
                    continue;
                }

                testeazaEtichetaSlash(etichetaStop,s);

                if(s.size() > 1)
                {
                    s.pop();
                }
            }
        }
        else
        {
            if(s.size() == 1) // daca nu se afla intr o eticheta
                schemaGresita = 1;
            else
            {
                nod* varf = s.top();
                if(verificaEticheta(varf, "start") || verificaEticheta(varf, "daca") || verificaEticheta(varf, "altfel") || verificaEticheta(varf, "altfelDaca")  || verificaEticheta(varf, "catTimp") || verificaEticheta(varf, "pentru") || verificaEticheta(varf, "catTimp") || verificaEticheta(varf, "doWhile"))
                {
                    if(c != ' ' && c != '\t' && c != '\n')
                        schemaGresita = 1;
                }else
                {
                    varf->info[varf->len++] = c;
                    varf->info[varf->len] = '\0';
                }
            }
        }
    }

    fclose(fin);
    if(s.top()->nrFii > 1) // avem o padure, nu un arbore, deci nu este un cod corect
        schemaGresita = 1;

    return s.top()->fiu[0];

}


void cautaMesaj(char eticheta[],char target[],char mesaj[])
{
    char* p = strstr(eticheta,target);
    int i = 0;
    if(strcmp(target,"cond=") == 0)
    {
        p+=5;
    }
    else if(strcmp(target,"i=") == 0)
    {
        p+=2;
    }
    else if(strcmp(target,"inc=") == 0)
    {
        p+=4;
    }

    int d = 0;
    while( (d != 0 || *p != ' ') && *p != '\0' && d >= 0)
    {
        if(*p == '(')
            d++;
        else if(*p == ')')
            d--;
        mesaj[i++] = *p;
        p++;

    }
    mesaj[i] = '\0';

}

void adaugaBlocSchema(int tip,int& x,int& y,nod* obj,int pred)
{
    if(pred != -1)
    {
        if(a[pred].tip == 4 && a[pred].st != -1)
        {
            a[pred].dr = nr_blocuri;
            x += 300;
        }
        else
            a[pred].st = nr_blocuri;
        y += 150;
    }

    if(tip == 0)
    {
        start_main = nr_blocuri;
        a[nr_blocuri] = {tip,1,x,y};
    }
    else if(tip == 1)
    {
        a[nr_blocuri] = {tip,1,x,y};
    }
    else
    {
        a[nr_blocuri] = {tip,1,x,y,obj->info};
    }
    a[nr_blocuri].ant.push_back(pred);
    if(tip != 1)
        obj->componenta = nr_blocuri;
    nr_blocuri++;
}

void creeazaSchema(nod* obiect, int& x,int& y,int pred)
{
    if (obiect!=NULL)
    {

        if(verificaEticheta(obiect,"calcul"))
        {
            adaugaBlocSchema(5,x,y,obiect,pred);
        }
        else if(verificaEticheta(obiect,"citeste"))
        {
            adaugaBlocSchema(2,x,y,obiect,pred);
        }
        else if(verificaEticheta(obiect,"scrie"))
        {
            adaugaBlocSchema(3,x,y,obiect,pred);
        }
        else
        {
            if(verificaEticheta(obiect,"start"))
            {
                adaugaBlocSchema(0,x,y,obiect,pred);
            }
            else if(verificaEticheta(obiect,"daca"))
            {
                char mesaj[NMAX];
                char aux[NMAX] = "(";

                cautaMesaj(obiect->eticheta,"cond=",mesaj);
                strcat(aux,mesaj);
                strcat(aux,")");
                strcpy(obiect->info,aux);
                adaugaBlocSchema(4,x,y,obiect,pred);
            }
            else if(verificaEticheta(obiect, "altfelDaca"))
            {
                char mesaj[NMAX];
                cautaMesaj(obiect->eticheta,"cond=",mesaj);
                strcat(mesaj," && !(");
                strcat(mesaj,a[pred].text);
                strcat(mesaj,")");
                strcpy(obiect->info,mesaj);
                adaugaBlocSchema(4,x,y,obiect,pred);
            }
            else if(verificaEticheta(obiect, "altfel"))
            {
                char aux[NMAX] = "";
                char mesaj[NMAX];
                strcat(aux,"!");
                strcat(aux,obiect->info);
                strcpy(obiect->info,aux);
                adaugaBlocSchema(4,x,y,obiect,pred);
            }
            else if(verificaEticheta(obiect, "catTimp"))
            {
                char mesaj[NMAX];
                cautaMesaj(obiect->eticheta,"cond=",mesaj);
                strcpy(obiect->info,mesaj);
                adaugaBlocSchema(4,x,y,obiect,pred);
            }
            else if(verificaEticheta(obiect, "pentru"))
            {
                char mesaj[NMAX];
                nod* aux = new nod;
                cautaMesaj(obiect->eticheta,"i=",mesaj);
                strcpy(aux->info,mesaj);
                adaugaBlocSchema(5,x,y,aux,pred);

                cautaMesaj(obiect->eticheta,"cond=",mesaj);
                strcpy(obiect->info,mesaj);
                adaugaBlocSchema(4,x,y,obiect,aux->componenta);
                pred = nr_blocuri;
            }
            else if(verificaEticheta(obiect, "doWhile"))
            {
                obiect->componenta = pred;
            }


            for(int i=0;i<obiect->nrFii;i++)
            {
                if(i == 0)
                {
                    creeazaSchema(obiect->fiu[i],x,y,obiect->componenta);
                }
                else
                {

                    if(a[obiect->fiu[i-1]->componenta].tip == 4 && (verificaEticheta(obiect->fiu[i-1]->eticheta,"daca") || verificaEticheta(obiect->fiu[i-1]->eticheta,"altfelDaca") ) && (verificaEticheta(obiect->fiu[i]->eticheta,"altfelDaca") || verificaEticheta(obiect->fiu[i]->eticheta,"altfel")) )
                    {
                        strcpy(obiect->fiu[i]->info,obiect->fiu[i-1]->info);
                    }
                    creeazaSchema(obiect->fiu[i],x,y,obiect->fiu[i-1]->componenta);
                }
            }

            if(verificaEticheta(obiect, "doWhile"))
            {
                char mesaj[NMAX];
                int saveComp;

                cautaMesaj(obiect->eticheta,"cond=",mesaj);
                strcpy(obiect->info,mesaj);

                if(obiect->nrFii)
                {
                    saveComp = obiect->fiu[0]->componenta;
                    adaugaBlocSchema(4,x,y,obiect,obiect->fiu[obiect->nrFii-1]->componenta);
                }
                else
                {
                    adaugaBlocSchema(4,x,y,obiect,pred);
                    saveComp = obiect->componenta;
                }
                a[obiect->componenta].st = saveComp;
                a[saveComp].ant.push_back(obiect->componenta);

            }
            else if(verificaEticheta(obiect, "catTimp"))
            {

                if(obiect->nrFii != 0)
                {

                    if(a[obiect->fiu[obiect->nrFii-1]->componenta].tip == 4)
                    {
                        a[obiect->fiu[obiect->nrFii-1]->componenta].dr = obiect->componenta;
                        a[obiect->componenta].ant.push_back(obiect->fiu[obiect->nrFii-1]->componenta);

                    }
                    else
                    {
                        a[obiect->fiu[obiect->nrFii-1]->componenta].st = obiect->componenta;
                        a[obiect->componenta].ant.push_back(obiect->fiu[obiect->nrFii-1]->componenta);
                    }

                }
                else
                {
                    a[obiect->componenta].st = obiect->componenta;
                    a[obiect->componenta].ant.push_back(obiect->componenta);
                }
            }
            else if(verificaEticheta(obiect, "pentru"))
            {
                nod* aux = new nod;
                char mesaj[NMAX];
                cautaMesaj(obiect->eticheta,"inc=",mesaj);
                strcpy(aux->info, mesaj);

                if(obiect->nrFii != 0)
                    adaugaBlocSchema(5,x,y,aux,obiect->fiu[obiect->nrFii-1]->componenta);
                else
                    adaugaBlocSchema(5,x,y,aux,obiect->componenta);

                a[aux->componenta].st = obiect->componenta;
                a[obiect->componenta].ant.push_back(aux->componenta);

            }
            else if(obiect->nrFii == 0)
                adaugaBlocSchema(1,x,y,obiect,obiect->componenta);
            else
                adaugaBlocSchema(1,x,y,obiect,obiect->fiu[obiect->nrFii-1]->componenta);

        }



    }
}


