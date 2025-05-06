#include <iostream>
#include <type_traits>
#include <string>
#include <chrono>

int colisoes = 0;

//NECESSÁRIO ATUALIZAR VERSÕES DO GCC >=7.x.x

template <typename thing>
class node {


public:node  *prev;
public:thing A;
public:node *next;

public :static node* new_node(thing T) {

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
    int size;

public:linked_list() {

        this->head=nullptr;
        this->tail=nullptr;
        this->iterator=nullptr;
        size=0;
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
        else {

            this->head->prev = S;
            S->prev = nullptr;
            S->next = this->head;
            this->head=S;
        }

        this->size+=1;

        if(this->size>1) {
            colisoes++;
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
        std::cout<<"NOT STRING";
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
    int N_Strings = 1000000;
    int k = rand()%N_Strings;
    double M_Insercao_NS = 0;
    double M_Insercao_MS =0;
    double M_Insercao_SEG =0;
    std::string strBuscar;


    /*
     *
     * EXEMPLO TESTE DE MESA = {"ALEXANDRE","APOLO","GUILHERME", "EDUARDO", "FAMO"}
     * M=3
     *
     *std::string V[5]= {"ALEXANDRE","APOLO","GUILHERME","EDUARDO","FAMO"};

     */


    hash_table<std::string> H = hash_table<std::string>( 31);





    for (int i =0 ; i<N_Strings; i++) {

        std::string item = genString(1000);

        if(i==k) {
            strBuscar=item;
        }



        auto inicio = std::chrono::high_resolution_clock::now();
        H.insert(item);
        auto fim = std::chrono::high_resolution_clock::now();

        auto duracao_nanosegundos = std::chrono::duration_cast<std::chrono::nanoseconds>(fim-inicio);
        auto duracao_milissegundos = std::chrono::duration_cast<std::chrono::milliseconds>(fim-inicio);
        auto duracao_segundos = std::chrono::duration_cast<std::chrono::seconds>(fim-inicio);

        std::cout<<"-----------------------------------\n";
        std::cout<<"INSERCAO NUMERO "<<i<<"\n";
        std::cout<<"STRING INSERIDA = {' "<<item<<" '}\n";
        std::cout<<"QUANTIDADE DE CHAR's = "<<item.size()<<"\n";
        std::cout<<"Tempo de execucao para insercao: "<<duracao_nanosegundos.count()<<"ns\n";
        std::cout<<"Tempo de execucao para insercao: "<<duracao_milissegundos.count()<<"ms\n";
        std::cout<<"Tempo de execucao para insercao: "<<duracao_segundos.count()<<"seg\n";

        M_Insercao_NS+=duracao_nanosegundos.count();
        M_Insercao_MS+=duracao_milissegundos.count();
        M_Insercao_SEG+=duracao_segundos.count();

    }


    auto inicio_busca = std::chrono::high_resolution_clock::now();
    H.search(strBuscar);
    auto fim_busca = std::chrono::high_resolution_clock::now();

    auto duracao_nanosegundos_busca = std::chrono::duration_cast<std::chrono::nanoseconds>(fim_busca-inicio_busca);
    auto duracao_milissegundos_busca = std::chrono::duration_cast<std::chrono::milliseconds>(fim_busca-inicio_busca);
    auto duracao_segundos_busca = std::chrono::duration_cast<std::chrono::seconds>(fim_busca-inicio_busca);


    auto inicio_remocao = std::chrono::high_resolution_clock::now();
    H.remove(strBuscar);
    auto fim_remocao = std::chrono::high_resolution_clock::now();

    auto duracao_nanosegundos_remocao = std::chrono::duration_cast<std::chrono::nanoseconds>(fim_remocao-inicio_remocao);
    auto duracao_milissegundos_remocao = std::chrono::duration_cast<std::chrono::milliseconds>(fim_remocao-inicio_remocao);
    auto duracao_segundos_remocao = std::chrono::duration_cast<std::chrono::seconds>(fim_remocao-inicio_remocao);

    std::cout<<"-----------------------------------\n\n";
    std::cout<<"Numero de colisoes totais= "<<colisoes<<"\n";
    std::cout<<"MEDIA DE TEMPO DE INSERCAO\n";
    std::cout<<"Nanosegundos ="<<M_Insercao_NS/N_Strings<<"\n";
    std::cout<<"Milissegundos ="<<M_Insercao_MS/N_Strings<<"\n";
    std::cout<<"Segundos ="<<M_Insercao_SEG/N_Strings<<"\n";

    std::cout<<"-----------------------------------\n";
    std::cout<<"-----------------------------------\n";
    std::cout<<"-----------------------------------\n";
    std::cout<<"-----------------------------------\n";

    std::cout<<"STRING A SER BUSCADA = {' "<<strBuscar<<" '}\n";
    std::cout<<"QUANTIDADE DE CHAR's DA STRING ="<<strBuscar.size()<<"\n";
    std::cout<<"TEMPO DE BUSCA\n";
    std::cout<<"Nanossegundos="<<duracao_nanosegundos_busca.count()<<"\n";
    std::cout<<"Milissegundos="<<duracao_milissegundos_busca.count()<<"\n";
    std::cout<<"Segundos="<<duracao_segundos_busca.count()<<"\n";

    std::cout<<"-----------------------------------\n";
    std::cout<<"-----------------------------------\n";
    std::cout<<"-----------------------------------\n";
    std::cout<<"-----------------------------------\n";

    std::cout<<"STRING A SER REMOVIDA = {' "<<strBuscar<<" '}\n";
    std::cout<<"QUANTIDADE DE CHAR's DA STRING ="<<strBuscar.size()<<"\n";
    std::cout<<"TEMPO DE BUSCA\n";
    std::cout<<"Nanossegundos="<<duracao_nanosegundos_remocao.count()<<"\n";
    std::cout<<"Milissegundos="<<duracao_milissegundos_remocao.count()<<"\n";
    std::cout<<"Segundos="<<duracao_segundos_remocao.count()<<"\n";








}
