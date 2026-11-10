#include <iostream>
#include <vector>
#include <random>
#include <iomanip>
#include <string>


void print_matrix(const std::vector<std::vector<int>>& matrix){
    for(size_t i=0;i<matrix.size();++i){
        for(size_t j=0;j<matrix[i].size();++j){
            std::cout<<std::setw(3)<<matrix[i][j];
        }
        std::cout<<'\n';
    }
}

std::vector<std::vector<int>> gen_matrix_s(std::mt19937& gen,bool directed){
    std::uniform_int_distribution<int> distrib_size(2,10);
    int n=distrib_size(gen);
    std::vector<std::vector<int>> matrix(n,std::vector<int>(n,0));
    std::uniform_int_distribution<int> distrib_edge(0,directed?2:1);

    for(size_t i=0;i<matrix.size();++i){
        for(size_t j=i+1;j<matrix.size();++j){
            int value=distrib_edge(gen);
            if(!directed){
                matrix[i][j]=value;
                matrix[j][i]=value;
            }else if(value==1){
                matrix[i][j]=1; 
            }else if(value==2){
                matrix[j][i]=1; 
            }
        }
    }
    return matrix;
}

size_t size_s(const std::vector<std::vector<int>>& matrix,bool directed){
    size_t count=0;
    for(size_t i=0;i<matrix.size();++i){
        for(size_t j=0;j<matrix[i].size();++j){
            if(matrix[i][j]==1)++count;
        }
    }
    return directed?count:count/2;
}

size_t size_i(const std::vector<std::vector<int>>& matrix){
    return matrix.empty()?0:matrix[0].size();
}

std::vector<std::vector<int>> gen_matrix_i(
    const std::vector<std::vector<int>>& matrix_s,bool directed){
    size_t n=matrix_s.size();
    size_t m=size_s(matrix_s,directed);
    std::vector<std::vector<int>> matrix_i(n,std::vector<int>(m,0));
    size_t edge=0;

    for(size_t i=0;i<n;++i){
        for(size_t j=0;j<n;++j){
            if(matrix_s[i][j]==0)continue;
            if(!directed && j<=i)continue;
            matrix_i[i][edge]=directed?-1:1;
            matrix_i[j][edge]=1;
            ++edge;
        }
    }
    return matrix_i;
}

// incidence=false: матрица смежности; true: матрица инцидентности.
void type_v(const std::vector<std::vector<int>>& matrix,
            bool directed,bool incidence){
    if(matrix.empty())return;
    bool found=false;
    size_t n=matrix.size();

    for(size_t i=0;i<n;++i){
        size_t out=0,in=0;
        for(size_t j=0;j<matrix[i].size();++j){
            if(!directed){
                if(matrix[i][j]==1)++out;
            }else if(incidence){
                if(matrix[i][j]==-1)++out;
                if(matrix[i][j]==1)++in;
            }else{
                if(matrix[i][j]==1)++out;
                if(matrix[j][i]==1)++in;
            }
        }

        if(directed){
            std::cout<<"Вершина "<<i+1<<": исходящих "<<out
                     <<", входящих "<<in<<'\n';
        }
        if(out+in==0){
            std::cout<<"Вершина "<<i+1<<" ИЗОЛИРОВАНА\n";
            found=true;
        }
        if(out+in==1){
            std::cout<<"Вершина "<<i+1<<" КОНЦЕВАЯ\n";
            found=true;
        }
        if(n>1 && out==n-1){
            std::cout<<"Вершина "<<i+1<<" ДОМИНИРУЮЩАЯ\n";
            found=true;
        }
        if(directed && in==0 && out>0){
            std::cout<<"Вершина "<<i+1<<" ИСТОЧНИК\n";
            found=true;
        }
        if(directed && out==0 && in>0){
            std::cout<<"Вершина "<<i+1<<" СТОК\n";
            found=true;
        }
    }
    if(!found)std::cout<<"ОСОБЕННЫЕ ВЕРШИНЫ НЕ НАЙДЕНЫ\n";
}

int main(int argc,char* argv[]){
    if(argc!=2 || (std::string(argv[1])!="0" && std::string(argv[1])!="1")){
        std::cout<<"Запуск: "<<argv[0]<<" 0 или "<<argv[0]<<" 1\n"
                 <<"0 - неориентированный граф, 1 - ориентированный\n";
        return 1;
    }

    bool directed=argv[1][0]=='1';
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<std::vector<int>> matrix_s=gen_matrix_s(gen,directed);
    std::vector<std::vector<int>> matrix_i=gen_matrix_i(matrix_s,directed);

    std::cout<<(directed?"ОРИЕНТИРОВАННЫЙ ГРАФ\n":"НЕОРИЕНТИРОВАННЫЙ ГРАФ\n");
    std::cout<<"КОЛИЧЕСТВО ВЕРШИН: "<<matrix_s.size()<<'\n';
    std::cout<<"КОЛИЧЕСТВО РЕБЕР / ДУГ: "<<size_s(matrix_s,directed)<<'\n';
    std::cout<<"\nМАТРИЦА СМЕЖНОСТИ:\n";
    print_matrix(matrix_s);
    std::cout<<"\nОСОБЕННЫЕ ВЕРШИНЫ ПО МАТРИЦЕ СМЕЖНОСТИ:\n";
    type_v(matrix_s,directed,false);

    std::cout<<"\nМАТРИЦА ИНЦИДЕНТНОСТИ:\n";
    print_matrix(matrix_i);
    std::cout<<"КОЛИЧЕСТВО РЕБЕР / ДУГ: "<<size_i(matrix_i)<<'\n';
    std::cout<<"\nОСОБЕННЫЕ ВЕРШИНЫ ПО МАТРИЦЕ ИНЦИДЕНТНОСТИ:\n";
    type_v(matrix_i,directed,true);
    return 0;
}
