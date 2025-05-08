#include <iostream>
#include <type_traits>
#include <string>
#include <chrono>
#include <fstream>

//NECESSÁRIO ATUALIZAR VERSÕES DO GCC >=7.x.x


template <typename thing>
class node {


public:node  *prev;
public:thing A;
public:node *next;

public:static node* new_node(thing T) {

        node *N = new node();
        N->next=nullptr;
        N->prev=nullptr;
        N->A = T;

        return N;
    }

};

template <typename thing>
class linked_list {

    node<thing> *head;
    node <thing> *tail;
    node <thing> *iterator;
    int colission;
    int size;



public: linked_list() {

    this->head=nullptr;
    this->tail=nullptr;
    this->iterator=nullptr;
    this->size=0;
    this->colission=0;
}

public:bool insert_head(thing T) {

    node<thing> *S= node<thing>::new_node(T);
    if (this->head==nullptr && this->tail==nullptr) {

        this->head = S;
        this->tail = S;
        S->next=nullptr;
        S->prev=nullptr;
        this->iterator = head;
    }
    else if (this->head == this->tail) {

            this->head->prev = S;
            S->prev=nullptr;
            S->next=this->head;
            this->head = S;


    }
    else{
        this->head->prev = S;
        S->prev = nullptr;
        S->next = this->head;
        this->head=S;
    }

    this->size+=1;

    if(this->size>1) {
        this->colission+=1;
    }


    return true;
}


public:bool delete_iterator() {

    if (this->tail== this->head) {

        this->tail= nullptr;
        this->tail->next=nullptr;
        this->tail->prev=nullptr;


        this->head=nullptr;
        this->head->next= nullptr;
        this->head->prev=nullptr;

        delete iterator;
        this->iterator = nullptr;
        this->iterator->next=nullptr;
        this->iterator->prev=nullptr;
    }
    else if (this->iterator==this->tail) {

        this->iterator->prev->next = nullptr;
        this->tail= iterator->prev;
        delete iterator;
        this->iterator=tail;

    }
    else if (this->iterator==this->head) {

        this->iterator->next->prev= nullptr;
        this->head = this->iterator->next;
        delete iterator;
        this->iterator=head;

    }
    else {

        iterator->prev->next= iterator->next;
        iterator->next->prev= iterator->prev;
        delete iterator;
    }

    this->size-=1;
    return true;

}


public:bool setIteratorHead() {

    this->iterator=this->head;
    return true;
}

public:bool incrementIterator() {

    this->iterator = this->iterator->next;
    if (this->iterator==nullptr) {
          return false;
    }
    return  true;
}

public:thing getHeadValue() {

    return this->head->A;
}

public:thing getIteratorValue() {

    return this->iterator->A;
}

public:int getColission() {
    return  this->colission;
}


public:bool clear() {

    this->iterator=this->head;
    while(this->iterator) {

        node<thing> *temp = this->iterator;
        this->iterator = this->iterator->next;
        delete temp;
    }

    this->iterator=nullptr;
    this->head=nullptr;
    this->tail=nullptr;
    this->size=0;
    this->colission=0;

    return true;
}



    ~linked_list() {

      this->iterator=this->head;

       while(this->iterator) {
           node<thing> *temp = this->iterator;
           this->iterator = this->iterator->next;
           delete temp;
       }

       this->iterator=nullptr;
       this->head=nullptr;
       this->tail=nullptr;
}




};




template <typename thing>
class hash_table {

public:linked_list<thing> *V;
    int M;

    hash_table(int M) {
        this->V = new linked_list<thing>[M];
        this->M=M;
    }

public:bool insert(thing A) {

    int pos;

    if constexpr(std::is_same<thing, std::string>::value) {
        int j=0;

        for(int i =0  ; i<A.size() ; i++) {

            j+= A[i];
        }

        pos= j%this->M;
    }
    else {
        pos = A%this->M;
    }

    (V+pos)->insert_head(A);
    return  true;
}

public:bool search(thing A) {

    int pos;

    if constexpr (std::is_same<thing, std::string>::value) {

        int j=0;

        for(int i =0  ; i<A.size() ; i++) {

            j+= A[i];
        }

        pos= j%this->M;

    }
    else {
        pos = A%this->M;
    }


    (V+pos)->setIteratorHead();
    thing aux = (V+pos)->getIteratorValue();

    while (A!= aux) {

        if ((V+pos)->incrementIterator()) {
            aux = (V+pos)->getIteratorValue();
        }
        else {
            return false;
        }
    }

    return true;
}

public:bool remove(thing A) {

    int pos;

    if constexpr (std::is_same<thing, std::string>::value) {

        int j=0;

        for(int i =0  ; i<A.size() ; i++) {

            j+= A[i];
        }

        pos= j%this->M;

    }
    else {
        pos = A%this->M;
    }

    (V+pos)->setIteratorHead();
    thing aux = (V+pos)->getIteratorValue();

    while (A!= aux) {
        if ((V+pos)->incrementIterator()) {
            aux = (V+pos)->getIteratorValue();
        }
        else {
            return false;
        }
    }


    (V+pos)->delete_iterator();

    return true;

}

public:bool clear() {

    for(int i = 0 ; i<this->M ; i++) {

        (V+i)->clear();
    }

    return true;
}

public:int getColissionPerLine() {

    int T = 0;

    for(int i =0,temp ; i<this->M ; i++) {

        temp = (V+i)->getColission();
        std::cout<<"V["<<i<<"] = "<<temp<<" colisoes\n";
        T+=temp;
    }

    return T;
}


    ~hash_table() {

    for(int i =0 ; i<this->M ; i++) {

        (V+i)->clear();
    }

    delete []V;

}




};


char genCharByASCII() {

    return rand()%94+ 33;

}


std::string genString(int maxSize) {

    int sizeString = rand()%maxSize+1;
    char c;
    std::string newString;


    for(int i =0 ; i<sizeString; i++) {

        c=genCharByASCII();
        newString.push_back(c);
    }

    return newString;
}


int main() {


    srand (time(NULL));
    int N_Strings = 100;
    int k;
    double T_Insercao_H31 = 0;
    double T_Insercao_H79 =0;
    double T_Insercao_H151 =0;
    int N_Colisoes_H31;
    int N_Colisoes_H79;
    int N_Colisoes_H151;
    std::string strBuscar;






    /*
     *
     * EXEMPLO TESTE DE MESA = {"ALEXANDRE","APOLO","GUILHERME", "EDUARDO", "FAMO"}
     * M=3
     *
     *std::string V[5]= {"ALEXANDRE","APOLO","GUILHERME","EDUARDO","FAMO"};

     */


    hash_table<std::string> H31 = hash_table<std::string>(31);
    hash_table<std::string> H79 = hash_table<std::string>(79);
    hash_table<std::string> H151 = hash_table<std::string>(151);

    std::ofstream out("dados.txt");

    if(!out.is_open()) {
        std::cout<<"Erro ao abrir arquivo";
        return -1;
    }



    while(N_Strings<100000) {
        for(int j = 0 ; j<10 ; j++) {


            k = rand()%N_Strings;

            for (int i =0 ; i<N_Strings; i++) {

                std::string item = genString(1000);

                auto inicio_insercao_H31 = std::chrono::high_resolution_clock::now();
                H31.insert(item);
                auto fim_insercao_H31 = std::chrono::high_resolution_clock::now();

                auto inicio_insercao_H79 = std::chrono::high_resolution_clock::now();
                H79.insert(item);
                auto fim_insercao_H79 = std::chrono::high_resolution_clock::now();

                auto inicio_insercao_H151 = std::chrono::high_resolution_clock::now();
                H151.insert(item);
                auto fim_insercao_H151 = std::chrono::high_resolution_clock::now();

                auto duracao_nanosegundos_insercao_H31 = std::chrono::duration_cast<std::chrono::nanoseconds>(fim_insercao_H31-inicio_insercao_H31);
                auto duracao_nanosegundos_insercao_H79 = std::chrono::duration_cast<std::chrono::nanoseconds>(fim_insercao_H79-inicio_insercao_H79);
                auto duracao_nanosegundos_insercao_H151 = std::chrono::duration_cast<std::chrono::nanoseconds>(fim_insercao_H151-inicio_insercao_H151);

                T_Insercao_H31+=duracao_nanosegundos_insercao_H31.count();
                T_Insercao_H79+=duracao_nanosegundos_insercao_H79.count();
                T_Insercao_H151+=duracao_nanosegundos_insercao_H151.count();



                if(i==k) {
                    strBuscar=item;
                }


            }

            T_Insercao_H31/=N_Strings;
            T_Insercao_H79/=N_Strings;
            T_Insercao_H151/=N_Strings;


            auto inicio_busca_H31 = std::chrono::high_resolution_clock::now();
            H31.search(strBuscar);
            auto fim_busca_H31 = std::chrono::high_resolution_clock::now();

            auto inicio_busca_H79 = std::chrono::high_resolution_clock::now();
            H31.search(strBuscar);
            auto fim_busca_H79 = std::chrono::high_resolution_clock::now();

            auto inicio_busca_H151 = std::chrono::high_resolution_clock::now();
            H31.search(strBuscar);
            auto fim_busca_H151 = std::chrono::high_resolution_clock::now();



            auto duracao_nanosegundos_busca_H31 = std::chrono::duration_cast<std::chrono::nanoseconds>(fim_busca_H31-inicio_busca_H31);
            auto duracao_nanosegundos_busca_H79 = std::chrono::duration_cast<std::chrono::nanoseconds>(fim_busca_H79-inicio_busca_H79);
            auto duracao_nanosegundos_busca_H151 = std::chrono::duration_cast<std::chrono::nanoseconds>(fim_busca_H151-inicio_busca_H151);


            N_Colisoes_H31 = H31.getColissionPerLine();
            N_Colisoes_H79 = H79.getColissionPerLine();
            N_Colisoes_H151= H151.getColissionPerLine();



            out<<N_Strings<<"|31|"<<N_Colisoes_H31<<"|"<<duracao_nanosegundos_busca_H31.count()<<"|"<<T_Insercao_H31<<"\n";
            out<<N_Strings<<"|79|"<<N_Colisoes_H79<<"|"<<duracao_nanosegundos_busca_H79.count()<<"|"<<T_Insercao_H79<<"\n";
            out<<N_Strings<<"|151|"<<N_Colisoes_H151<<"|"<<duracao_nanosegundos_busca_H151.count()<<"|"<<T_Insercao_H151<<"\n";
            H31.clear();
            H79.clear();
            H151.clear();


        }

        N_Strings*=10;

    }

}
