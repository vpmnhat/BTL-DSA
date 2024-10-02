#include "main.h"

int maxSize,curTime=1;

int nCr(int n, int k) {
	vector<vector<int>> dp(n + 1, vector<int>(k + 1, 0));
	for (int i = 0; i <= n; i++) {
		dp[i][0] = 1;
		if (i <= k) { dp[i][i] = 1; }
	}

	for (int i = 0; i <= n; i++) {
		for (int j = 1; j <= min(i, k); j++) {
			if (i != j) {  
				dp[i][j] = (dp[i - 1][j - 1] + dp[i - 1][j]) % maxSize;
			}
		}
	}
	return dp[n][k]; 
}

int countPerm(vector<int>& v)
{
    int n = v.size();
    if (n <= 2) return 1;
    vector<int> left, right;
    int root = v[0];
    for (int i = 1; i < n; i++) {
        if (v[i] < root) left.push_back(v[i]);
        else right.push_back(v[i]);
    }
    return nCr(n-1,left.size()) * countPerm(left) % maxSize * countPerm(right) % maxSize;
}

class HuffTree{
public:
    class Node{
    public:
        int key;  
        Node *left;  
        Node *right;  
        char c;

        Node(int key, char c){
            this->key=key;
            left=right=nullptr;
            this->c=c;
        }
    };

    int height(Node *p){
        if (!p) return 0;
        return max(height(p->left),height(p->right))+1;
    } 

    void rightRotate(Node* &root){  
        Node* temp = root->left;
        root->left = temp->right;
        temp->right = root;
        root = temp;
    }

    void leftRotate(Node* &root){  
        Node* temp = root->right;
        root->right = temp->left;
        temp->left = root;
        root = temp;
    }  

    int getBalance(Node *p){  
        if (!p) return 0;
        return height(p->left) - height(p->right);  
    }  

    Node* root;
    int numOfRotate;

    void copyTree(const Node* src, Node* &dest){
        if (!src) dest=nullptr;
        else {
            dest=new Node(src->key,src->c);
            copyTree(src->left,dest->left);
            copyTree(src->right,dest->right);
        }
    }

    HuffTree(int key,char c){
        root=new Node(key,c);
    }

    HuffTree(HuffTree* src){
        copyTree(src->root,root);
    }

    void balance(Node* &root){
        if (numOfRotate>=3) return;
        if (getBalance(root)>1){
            if (getBalance(root->left)>=0) rightRotate(root);
            else {
                leftRotate(root->left);
                rightRotate(root);
            }
            numOfRotate++;
        }
        else if (getBalance(root)<-1){
            if (getBalance(root->right)<=0) leftRotate(root);
            else {
                rightRotate(root->right);
                leftRotate(root);
            }
            numOfRotate++;
        }
        if (abs(getBalance(root->left))>1) balance(root->left);
        if (abs(getBalance(root->right))>1) balance(root->right);
        if (abs(getBalance(root))>1) balance(root);
    }


    HuffTree(HuffTree* t1, HuffTree* t2){
        root=new Node(t1->root->key+t2->root->key,0);
        copyTree(t1->root,root->left);
        copyTree(t2->root,root->right);       
        numOfRotate=0;
        balance(root);
    }

    void createCode(Node* root,string s,map<char,string> &m){
        if (!root) return;
        if (isalpha(root->c)) m[root->c]=s;
        createCode(root->left,s+"0",m);
        createCode(root->right,s+"1",m);
    }

    map<char,string> createCode(){
        map<char,string> m;
        if (!root) return m;
        if (!root->left&&!root->right){
            m[root->c]="0";
            return m;
        }
        createCode(root,"",m);
        return m;
    }

    void hand(Node* root){
        if (!root) return;
        hand(root->left);
        if (root->c!=0) cout<<root->c<<'\n';
        else cout<<root->key<<'\n';
        hand(root->right);
    }

    void hand(){
        hand(root);
    }

    void del(Node* p){
        if (!p) return;
        del(p->left);
        del(p->right);
        delete p;
    }

    ~HuffTree(){
        del(root);
    }
};

class BST{
public:
    class Node{
    public:
        int value;
        Node *pLeft,*pRight;
        Node(int value) : value(value), pLeft(NULL), pRight(NULL) {}
    };

    Node* root;
    queue<int> order;

    BST(){
        root=nullptr;
    }

    void add(Node*& p,int x){
        if (!p) p=new Node(x);
        else if (x<p->value) add(p->pLeft,x);
        else add(p->pRight,x);
    }

    void add(int value){
        add(root,value);
        order.push(value);
    }

    void del(Node*& p,int x){
        if (!p) return;
        if (p->value>x) del(p->pLeft,x);
        else if (p->value<x) del(p->pRight,x);
        else {
            if (!p->pLeft&&!p->pRight){
                delete p;
                p=nullptr;
            }
            else if (!p->pLeft&&p->pRight){
                Node* t=p;
                p=p->pRight;
                delete t;
            }
            else if (p->pLeft&&!p->pRight){
                Node* t=p;
                p=p->pLeft;
                delete t;
            }
            else {
                Node *t=p->pRight;
                while (t->pLeft) t=t->pLeft;
                p->value=t->value;
                del(p->pRight,p->value);
            }
        }
    }

    void del(int value){
        del(root,value);
    }

    vector<int> bfsOrder(){
        if (!root) return {};
        queue<Node*> q;
        vector<int> v;
        q.push(root);
        while (!q.empty()){
            Node* p=q.front();
            q.pop();
            v.push_back(p->value);
            if (p->pLeft) q.push(p->pLeft);
            if (p->pRight) q.push(p->pRight);
        }
        return v;
    }

    void kokusen(){
        if (!root) return;
        vector<int> v=bfsOrder();
        int y=countPerm(v)%maxSize;
        for (int i=0;i<y&&!order.empty();i++){
            del(order.front());
            order.pop();
        }
    }

    void printInorder(Node* root){
        if (!root) return;
        printInorder(root->pLeft);
        cout<<root->value<<'\n';
        printInorder(root->pRight);
    }

    void limitless(){
        printInorder(root);
    }

    ~BST(){
        while (root) del(root->value);
    }
};

class MinHeap{
public:
    class Node{
    public:
        int numOfCus, timeIn, timeUsed;
        deque<int> order;
        Node(){
            numOfCus=0;
            timeUsed=timeIn=-1;
        }
    };

    vector<Node> vNode;
    int numOfNode;
    vector<int> heap;

    MinHeap(){
        vNode.resize(maxSize);
        numOfNode=0;
        heap.resize(maxSize);
    }

    bool cmp(int x,int y){
        int a=vNode[heap[x]-1].numOfCus;
        int b=vNode[heap[y]-1].numOfCus;
        int c=vNode[heap[x]-1].timeUsed;
        int d=vNode[heap[y]-1].timeUsed;
        return a<b||(a==b&&c<d);
    }

    void reheapUp(int id){
        while (id){
            int p=(id-1)/2;
            if (cmp(p,id)) return;
            swap(heap[p],heap[id]);
            id=p;
        }
    }

    void reheapDown(int id){
        while (id<numOfNode){
            int c=id*2+1;
            if (c>=numOfNode) return;
            if (c+1<numOfNode&&cmp(c+1,c)) c++;
            if (cmp(id,c)) return;
            swap(heap[id],heap[c]);
            id=c;
        }
    }

    int find(int val){
        for (int i=0;i<numOfNode;i++){
            if (heap[i]==val) return i;
        }
        return -1;
    }

    void add(int val){
        int id=val%maxSize;
        if (vNode[id].numOfCus==0){
            vNode[id].numOfCus++;
            vNode[id].timeIn=curTime;
            vNode[id].timeUsed=curTime;
            numOfNode++;
            heap[numOfNode-1]=id+1;
            reheapUp(numOfNode-1);
        }
        else {
            int x=find(id+1);
            assert(x!=-1);
            vNode[id].numOfCus++;
            vNode[id].timeUsed=curTime;
            reheapDown(x);
        }
        vNode[id].order.push_back(val);
    }

    vector<int> least(int num){
        num=min(num,numOfNode);
        vector<int> res;
        vector<pair<pair<int,int>,int>> v;
        for (int i=0;i<numOfNode;i++){
            int x=heap[i];
            v.push_back({{vNode[x-1].numOfCus,vNode[x-1].timeUsed},x});
        }
        sort(v.begin(),v.end());
        for (int i=0;i<num;i++) res.push_back(v[i].second);
        return res;
    }

    void keitenken(int num){
        vector<int> v=least(num);
        for (int x:v){
            int id=find(x);
            assert(id!=-1);
            if (num>=vNode[x-1].numOfCus){
                for (int y:vNode[x-1].order){
                    cout<<y<<'-'<<x<<'\n';
                    vNode[x-1].order.pop_front();
                }
                vNode[x-1].numOfCus=0;
                vNode[x-1].timeIn=vNode[x-1].timeUsed=-1;
                reheapUp(id);
                heap[0]=heap[numOfNode-1];
                numOfNode--;
                reheapDown(0);
            }
            else {
                for (int i=0;i<num;i++){
                    cout<<vNode[x-1].order.front()<<'-'<<x<<'\n';
                    vNode[x-1].order.pop_front();
                }
                vNode[x-1].numOfCus-=num;
                vNode[x-1].timeUsed=curTime;
                reheapUp(id);
            }
        }
    }

    void preOrder(int id, int num){
        if (id>=numOfNode) return;
        int x=min(num,vNode[heap[id]-1].numOfCus);
        deque<int> dq=vNode[heap[id]-1].order;
        for (auto it=dq.rbegin(); it!= dq.rbegin()+x; it++){
            cout<<heap[id]<<'-'<<*it<<'\n';
        }
        preOrder(2*id+1,num);
        preOrder(2*id+2,num);
    }

    void cleave(int num){
        preOrder(0,num);
    }
};

bool validName(string name){
    set<char> s;
    for (char c:name) s.insert(c);
    return s.size()>=3;
}

char caesar(char c,int n){
    if (isupper(c)) return (c+n-65)%26+65;
    else return (c+n-97)%26+97;
}

bool cmp(const pair<char,int> &x, const pair<char,int> &y){
    if (x.second!=y.second) return x.second<y.second;
    char a=x.first,b=y.first;
    return (a>=97&&b<97)||(a>=97&&b>=97&&a<b)||(a<97&&b<97&&a<b);
}

vector<pair<char,int>> listFromName(string name){
    map<char,int> m1,m2;
    for (char c:name) m1[c]++;
    for (auto &x:m1)  m2[caesar(x.first,x.second)]+=x.second;
    vector<pair<char,int>> v;
    for (auto &x:m2) v.push_back({x.first,x.second});
    sort(v.begin(),v.end(),cmp);
    return v;
}

class Compare{
public:
    bool operator()(pair<HuffTree*,int> p1,pair<HuffTree*,int> p2){
        int a=p1.first->root->key,b=p2.first->root->key;
        if (a!=b) return a>b;
        return p1.second>p2.second;
    }
};

void simulate(string filename)
{
	ifstream ifs(filename);
    string str;
    int num;
    ifs>>str;
    ifs>>maxSize;
    vector<BST> gojoRes(maxSize);
    MinHeap sakunaRes;
    HuffTree* newCus=nullptr;
    while (ifs >> str){
        curTime++;
        //cout<<"*******LENH THU "<<curTime<<"**********\n";
        if (str=="LAPSE"){
            ifs>>str;
            if (!validName(str)) continue;
            vector<pair<char,int>> v=listFromName(str);
            //for (auto &p:v) cout<<p.first<<' '<<p.second<<'\n';
            priority_queue<pair<HuffTree*,int>,vector<pair<HuffTree*,int>>,Compare> pq;
            int t=0;
            for (auto &x:v){
                pq.push({new HuffTree(x.second,x.first),t});
                t++;
            }    
            while (pq.size()>1){
                HuffTree* t1=new HuffTree(pq.top().first);
                delete pq.top().first;
                pq.pop();
                HuffTree* t2=new HuffTree(pq.top().first);
                delete pq.top().first;
                pq.pop();
                pq.push({new HuffTree(t1,t2),t});
                delete t1;
                delete t2;
                t++;
            }
            delete newCus;
            newCus=new HuffTree(pq.top().first);
            delete pq.top().first;
            map<char,string> m=newCus->createCode();
            map<char,int> m1;
            string s;
            for (char c:str) m1[c]++;
            for (char c:str){
                s+=m[caesar(c,m1[c])];
            }
            reverse(s.begin(),s.end());
            if (s.size()>10) s.resize(10);
            int result=0;
            for (int i=0,n=s.size();i<n;i++){
                if (s[i]=='1') result+=1<<(n-1-i);
            }

            //cout<<result<<'\n';
            
            if (result%2){
                int id=result%maxSize;
                gojoRes[id].add(result);
            }
            else {
                sakunaRes.add(result);
            }
        }
        else if (str=="KOKUSEN"){
            for (int i=0;i<maxSize;i++) gojoRes[i].kokusen();
        }
        else if (str=="KEITEIKEN"){
            ifs>>num;
            sakunaRes.keitenken(num);
        }
        else if (str=="HAND"){
            if (newCus) newCus->hand();
        }
        else if (str=="LIMITLESS"){
            ifs>>num;
            if (num>=1&&num<=maxSize) gojoRes[num-1].limitless();
        }
        else if (str=="CLEAVE"){
            ifs>>num;
            sakunaRes.cleave(num);
        }
    }

    gojoRes.clear();
    delete newCus;
}