#include "main.h"

class imp_res : public Restaurant
{
	public:	
		class Table{
		public:
			customer *newCus;
			int size;

			Table(){
				newCus=nullptr;
				size=0;
			}

			void addRed1(string name,int energy){
				customer *cus=new customer(name,energy,nullptr,nullptr);
				if (!newCus){
					newCus=cus;
					newCus->next=newCus;
					newCus->prev=newCus;
				}
				else if (energy>=newCus->energy){
					customer *nxt=newCus->next;
					cus->next=nxt;
					cus->prev=newCus;
					newCus->next=cus;
					nxt->prev=cus;
					newCus=cus;
				}
				else {
					customer *pre=newCus->prev;
					cus->next=newCus;
					cus->prev=pre;
					newCus->prev=cus;
					pre->next=cus;
					newCus=cus;
				}
				size++;
			}

			void addRed2(string name,int energy){
				customer *cus=new customer(name,energy,nullptr,nullptr);
				if (!newCus){
					newCus=cus;
					newCus->next=newCus;
					newCus->prev=newCus;
				}
				else {
					customer *p=newCus, *ans=newCus;
					int res=energy-p->energy;
					p=p->next;
					while (p!=newCus){
						if (abs(energy-p->energy)>abs(res)){
							res=energy-p->energy;
							ans=p;
						}
						p=p->next;
					}
					if (res>=0){
						customer *nxt=ans->next;
						cus->next=nxt;
						cus->prev=ans;
						ans->next=cus;
						nxt->prev=cus;
						newCus=cus;
					}
					else {
						customer *pre=ans->prev;
						cus->next=ans;
						cus->prev=pre;
						ans->prev=cus;
						pre->next=cus;
						newCus=cus;
					}
				}
				size++;
			}

			void erase(customer* cus){
				if (size==1){
					delete cus;
					newCus=nullptr;
				}
				else if (size==2){
					newCus=cus->next;
					delete cus;
					newCus->prev=newCus;
					newCus->next=newCus;
				}
				else {
					bool right=cus->energy>0;
					customer *pre=cus->prev, *nxt=cus->next;
					delete cus;
					pre->next=nxt;
					nxt->prev=pre;
					if (right) newCus=nxt;
					else newCus=pre;
				}
				size--;
			}
			
			~Table(){
				while (size) erase(newCus);
			}
		};

		class Queue{
		public:
			customer *frontCus, *backCus;
			int size;

			Queue(){
				frontCus=nullptr;
				backCus=nullptr;
				size=0;
			}

			void add(string name, int energy){
				customer *cus=new customer(name,energy,nullptr,nullptr);
				if (!frontCus){
					frontCus=cus;
					backCus=cus;
				}
				else {
					backCus->next=cus;
					cus->prev=backCus;
					backCus=cus;
				}
				size++;
			}

			void erase(customer* cus){
				if (size==1){
					delete cus;
					frontCus=nullptr;
					backCus=nullptr;
				}
				else if (size==2){
					if (cus==frontCus){
						delete cus;
						frontCus=backCus;
						frontCus->prev=nullptr;
					}
					else {
						delete cus;
						backCus=frontCus;
						backCus->next=nullptr;
					}
				}
				else {
					if (cus==frontCus){
						frontCus=frontCus->next;
						delete cus;
						frontCus->prev=nullptr;						
					}
					else if (cus==backCus){
						backCus=backCus->prev;
						delete cus;
						backCus->next=nullptr;
					}
					else {
						customer *pre=cus->prev, *nxt=cus->next;
						delete cus;
						pre->next=nxt;
						nxt->prev=pre;
					}
				}
				size--;
			}

			customer* get(int id){
				customer *cus=frontCus;
				for (int i=0;i<id;i++) cus=cus->next;
				return cus;
			}

			void swap(int id1,int id2){
				customer *cus1=get(id1), *cus2=get(id2);
				string name=cus1->name;
				int energy=cus1->energy;
				cus1->name=cus2->name;
				cus1->energy=cus2->energy;
				cus2->name=name;
				cus2->energy=energy; 
			}

			~Queue(){
				while (size) erase(frontCus);
			}
		};

		class Node{
		public:
			string name;
			int energy, id;   //id 1 la o Table; 2 la o Queue
			Node *prev, *next;

			Node(string name, int energy,int id){
				this->name=name;
				this->energy=energy;
				this->id=id;
				this->next=nullptr;
				this->prev=nullptr;
			}
		};

		class All{
		public:
			Node *firstNode, *lastNode;
			int size;

			All(){
				firstNode=nullptr;
				lastNode=nullptr;
				size=0;
			}

			bool sameName(string name){
				Node* p=firstNode;
				while (p){
					if (name==p->name) return 1;
					p=p->next;
				}
				return 0;
			}

			void add(string name,int energy,int id){
				Node *node=new Node(name,energy,id);
				if (!firstNode){
					firstNode=node;
					lastNode=node;
				}
				else {
					lastNode->next=node;
					node->prev=lastNode;
					lastNode=node;
				}
				size++;
			}

			void erase(Node* node){
				if (size==1){
					delete node;
					firstNode=nullptr;
					lastNode=nullptr;
				}
				else if (size==2){
					if (node==firstNode){
						delete node;
						firstNode=lastNode;
						firstNode->prev=nullptr;
					}
					else {
						delete node;
						lastNode=firstNode;
						lastNode->next=nullptr;
					}
				}
				else {
					if (node==firstNode){
						firstNode=firstNode->next;
						delete node;
						firstNode->prev=nullptr;						
					}
					else if (node==lastNode){
						lastNode=lastNode->prev;
						delete node;
						lastNode->next=nullptr;
					}
					else {
						Node *pre=node->prev, *nxt=node->next;
						delete node;
						pre->next=nxt;
						nxt->prev=pre;
					}
				}
				size--;
			}

			void eraseOanLinh(){
				Node *node=firstNode;
				while (node){
					if (node->energy>0){
						node=node->next;
						continue;
					}
					Node *p=node;
					node=node->next;
					erase(p);
				}
			}

			void eraseThuatSu(){
				Node *node=firstNode;
				while (node){
					if (node->energy<0){
						node=node->next;
						continue;
					}
					Node *p=node;
					node=node->next;
					erase(p);
				}
			}

			~All(){
				while (size) erase(firstNode);
			}
		};

		Table *table;
		Queue *queue;
		All *all;
		int exchange;

		imp_res() {
			table=new Table();
			queue=new Queue();
			all=new All();
			exchange=0;
		};

		bool cmp(customer *c1, customer *c2){
			if (abs(c1->energy)!=abs(c2->energy)) return abs(c1->energy)>abs(c2->energy);
			Node *node=all->firstNode;
			int x=0,y=0,id=0;
			while (node){
				if (node->name==c1->name) x=id;
				if (node->name==c2->name) y=id;
				id++;
				node=node->next;
			}
			return x<y;
		}

		void insSort(int begin,int n,int inc){  // 0 5 1
			for (int i=inc;i<n;i+=inc){
				for (int j=i;(j>=inc)&&cmp(queue->get(begin+j),queue->get(begin+j-inc));j-=inc){
					queue->swap(begin+j,begin+j-inc);
					exchange++;
				}
			}
		}

		void shellSort(int n){
			for (int i=n/2;i>2;i/=2){
				for (int j=0;j<i;j++){
					insSort(j,n-j,i);
				}
			}
			insSort(0,n,1);
		}

		void RED(string name, int energy)
		{
			if (all->sameName(name) || energy==0) return;

			if (table->size==MAXSIZE){
				if (queue->size==MAXSIZE) return;

				queue->add(name,energy);
				all->add(name,energy,2);
				return;
			}

			if (table->size<MAXSIZE/2){
				table->addRed1(name,energy);
				all->add(name,energy,1);		
			}
			else {
				table->addRed2(name,energy);
				all->add(name,energy,1);
			}
			
		}
		void BLUE(int num)
		{
			if (num==0) return;
			num=min(num,table->size);
			while (num--){
				Node* node=all->firstNode;
				while (node->id!=1) node=node->next;
				string name=node->name;
				all->erase(node);
				customer* cus=table->newCus;
				while (cus->name!=name) cus=cus->next;
				table->erase(cus);
			}
			int z=MAXSIZE-table->size;
			for (int i=0;i<z;i++){
				Node* node=all->firstNode;
				customer *cus=queue->frontCus;
				if (!cus) break;
				while (node->name!=cus->name) node=node->next;
				node->id=1;
				queue->erase(cus);
				string name=node->name;
				int energy=node->energy;

				if (table->size<MAXSIZE/2){
					table->addRed1(name,energy);
				}
				else {
					table->addRed2(name,energy);
				}
			}
		}
		void PURPLE()
		{
			int res=-1,n=0,id=0;
			customer *cus=queue->frontCus;
			while (cus){
				id++;
				if (abs(cus->energy)>=res){
					res=abs(cus->energy);
					n=id;
				}
				cus=cus->next;
			}
			shellSort(n);
			BLUE(exchange%MAXSIZE);
			exchange=0;			
		}
		void REVERSAL()
		{
			Queue *q1=new Queue(), *q2=new Queue();
			customer *cus=table->newCus, *ptr=nullptr;
			if (!cus) return;
			string name=cus->name;
			do {
				if (cus->energy>0) q1->add(cus->name,cus->energy);
				else q2->add(cus->name,cus->energy);
				cus=cus->prev;	
			}
			while (cus!=table->newCus);
			cus=table->newCus;
			do {
				if (cus->energy>0){
					customer* p=q1->backCus;
					cus->name=p->name;
					cus->energy=p->energy;
					q1->erase(p);
				}
				else {
					customer* p=q2->backCus;
					cus->name=p->name;
					cus->energy=p->energy;
					q2->erase(p);
				}
				if (cus->name==name) ptr=cus;
				cus=cus->prev;
			}
			while (cus!=table->newCus);
			table->newCus=ptr;
			delete q1;
			delete q2;
		}
		void UNLIMITED_VOID()
		{
			if (table->size<4) return;
			int minSum=2e9,maxLen=-2e9,sum=0,len=0,x=0; 
			customer *cus=table->newCus, *start=nullptr, *end=nullptr; //x=1 la start truoc end; 2 la nguoc lai

			do {
				sum=len=0;
				customer *p=cus;
				do {
					sum+=p->energy;
					len++;
					if (len>=4&&(sum<minSum||sum==minSum&&len>=maxLen)){
						minSum=sum;
						maxLen=len;
						start=cus;
						end=p;
					}
					p=p->next;
				}
				while (p!=cus);
				cus=cus->next;
			}
			while (cus!=table->newCus);
	
			int res=2e9;
			customer *cusMin=nullptr;
			cus=start;
			do {
				if (cus->energy<res){
					res=cus->energy;
					cusMin=cus;
				}
				cus=cus->next;
			}
			while (cus!=end->next);

			if (cusMin==start){
				cus=start;
				do {
					cus->print();
					cus=cus->next;
				}
				while (cus!=end->next);
			}
			else {
				cus=cusMin;
				while (cus!=end->next){
					cus->print();
					cus=cus->next;
				}
				cus=start;
				while (cus!=cusMin){
					cus->print();
					cus=cus->next;
				}
			}
			
		}
		void DOMAIN_EXPANSION()
		{
			int s1=0,s2=0;
			Node *node=all->firstNode;
			while (node){
				int x=node->energy;
				if (x>0) s1+=x;
				else s2+=abs(x);
				node=node->next;
			}
			if (s1>=s2){
				node=all->firstNode;
				Queue *q=new Queue();
				while (node){
					if (node->energy>0){
						node=node->next;
						continue;
					}
					if (node->id==1){
						customer* cus=table->newCus;
						while (cus->name!=node->name) cus=cus->next;
						q->add(cus->name,cus->energy);
						table->erase(cus);
					}
					else {
						customer* cus=queue->frontCus;
						while (cus->name!=node->name) cus=cus->next;
						q->add(cus->name,cus->energy);
						queue->erase(cus);
					}
					node=node->next;
				}
				customer *p=q->backCus;
				while (p){
					p->print();
					p=p->prev;
				}
				delete q;
				all->eraseOanLinh();
			}
			else {
				node=all->firstNode;
				Queue *q=new Queue();
				while (node){
					if (node->energy<0){
						node=node->next;
						continue;
					}
					if (node->id==1){
						customer* cus=table->newCus;
						while (cus->name!=node->name) cus=cus->next;
						q->add(cus->name,cus->energy);
						table->erase(cus);
					}
					else {
						customer* cus=queue->frontCus;
						while (cus->name!=node->name) cus=cus->next;
						q->add(cus->name,cus->energy);
						queue->erase(cus);
					}
					node=node->next;
				}
				customer *p=q->backCus;
				while (p){
					p->print();
					p=p->prev;
				}
				delete q;
				all->eraseThuatSu();
			}
			int z=MAXSIZE-table->size;
			for (int i=0;i<z;i++){
				node=all->firstNode;
				customer *cus=queue->frontCus;
				if (!cus) break;
				while (node->name!=cus->name) node=node->next;
				node->id=1;
				queue->erase(cus);
				string name=node->name;
				int energy=node->energy;

				if (table->size<MAXSIZE/2){
					table->addRed1(name,energy);
				}
				else {
					table->addRed2(name,energy);
				}
			}
		}
		void LIGHT(int num)
		{
			if (num!=0){
				if (table->size==0) return;
				customer* cus=table->newCus;
				cus->print();
				if (num>0){
					cus=cus->next;
					while (cus!=table->newCus){
						cus->print();
						cus=cus->next;
					}
				}
				else {
					cus=cus->prev;
					while (cus!=table->newCus){
						cus->print();
						cus=cus->prev;
					}
				}
			}
			else {
				if (queue->size==0) return;
				customer* cus=queue->frontCus;
				while (cus){
					cus->print();
					cus=cus->next;
				}
			}
		}

		void printAll(){
			Node *node=all->firstNode;
			while (node){
				cout<<node->name<<' '<<node->energy<<' '<<node->id<<'\n';
				node=node->next;
			}
		}

		~imp_res(){
			delete table;
			delete queue;
			delete all;
		}
};