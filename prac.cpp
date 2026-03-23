#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <sstream> 
#include <set>

using namespace std;

struct Match{
    string hometeam;
    string HomeSquat;
    vector<string> homePlayers; 
    string HomeCountry, VisitorTeam, VisitorSquad, VisitorCountry;
    vector<string> visitorPlayers; 
    string Date,Score;
    int Spectators;
    string Stadium,StadiumCountry,League;

};

vector<string> splitSquat(string s) {
    vector<string> res;
    string tmp = "";
    for (char c : s) {
        if (c == ';') {
            if (!tmp.empty()) res.push_back(tmp);
            tmp = "";
        }
        else {
            tmp += c;
        }
    }
   
    if (!tmp.empty()) res.push_back(tmp);
    
    return res; 
}

vector <Match> readFile(string filename){
    ifstream f(filename);
    if(!f) cout << "Erorr";

    vector<Match> M;

    string s;

    getline(f,s);

    while(getline(f,s,',')){
        if (s.empty()) continue;

        Match Matches;

        //Matches.hometeam = s;

        getline(f,s,',');
        Matches.HomeSquat = s;
        Matches.homePlayers = splitSquat(s);

        getline(f,s,',');
        Matches.HomeCountry = s;

        getline(f,s,',');
        Matches.VisitorTeam = s;

        getline(f,s,',');
        Matches.VisitorSquad = s;
        Matches.visitorPlayers = splitSquat(s);

        getline(f,s,',');
        Matches.VisitorCountry = s;

        getline(f,s,',');
        Matches.Date= s;

        getline(f,s,',');
        Matches.Score = s;

        getline(f,s,',');
        try{
            if(s.empty()) Matches.Spectators = 0 ;
            else{
                s.erase(remove(s.begin(),s.end(),','), s.end());
                 Matches.Spectators = stoi(s);
            }
        }
        catch(...){
             Matches.Spectators = 0;
        }

        getline(f,s,',');
        Matches.Stadium= s;

        getline(f,s,',');
        Matches.StadiumCountry = s;

        getline(f,s);


        Matches.League = s;
      //  if (!s.empty() && s[s.size() - 1] == '\r') s.erase(s.size() - 1);
        M.push_back(Matches);

    }
    f.close();
    return M;
}

struct Node{
    string key;
    Match data;     
    Node *L, *R;
    int height;          
};

Node* newnode(Match m){
    Node* p = new Node;
    p -> key = m.League;
    p -> data = m;
    p -> L = p-> R = nullptr;
    return p;
}
 
Node* insertNode (Node * root, Match m){
    if(root == nullptr){
        return newnode(m);
    }
    if(m.League < root ->key){
        root -> L = insertNode(root ->L, m);
    }
    else {
        root -> R = insertNode(root->R, m);
    }
    return root;
}

void NLR(Node* root, vector<string>& res) {
    if (root == nullptr) return; 

    res.push_back(root->key);    
    NLR(root->L, res);           
    NLR(root->R, res);              
}

void LNR(Node* root, vector<string>& res) {
    if (root == nullptr) return; 

       
    LNR(root->L, res);    
    res.push_back(root->key);        
    LNR(root->R, res);              
}

Node* createBST(vector<Match> M){
    Node* root  = nullptr;
    for(int i = 0; i < M.size(); i++ ){
        root = insertNode(root, M[i]);
    }
    return root;
}

void traversal(Node* root, int level, vector<vector<string>> &results){
        if (!root) return ;
        if ( level == results.size()){
            results.push_back({});
        }
        results[level].push_back(root -> key);

        traversal(root->L, level+1,results);
        traversal(root->R, level+1,results);
        
    }
vector<vector<string>> levelOrder(Node* root) {
        vector<vector<string>> result;
        traversal(root, 0, result);
        return result;
    }


string getMax(Node* root) {
    if (root == nullptr) return "";
    while (root->R != nullptr) root = root->R; // Cực phải là Max
    return root->key;
}

string getMin(Node* root) {
    if (root == nullptr) return "";
    while (root->L != nullptr) root = root->L; // Cực trái là Min
    return root->key;
}


bool isValid(Node* root, string minVal, string maxVal) {
    if (root == nullptr) return true;
    
    if ((minVal != "" && root->key <= minVal) || (maxVal != "" && root->key >= maxVal))
        return false;

    return isValid(root->L, minVal, root->key) && 
           isValid(root->R, root->key, maxVal);
}

Node* minNode(Node* root){
    Node* cur = root;
    while(cur && cur -> L != nullptr){
        cur = cur -> L;
    }
    return cur;
}

Node* deleteNode(Node* root, string key){
    if(root == nullptr) return root;
    if(key < root -> key) root-> L = deleteNode(root -> L,key);
    else if( key > root -> key) root -> R = deleteNode(root -> R,key);
    else{
        if(root -> L== nullptr) {
            Node* tmp = root-> R;
            delete root;
            return tmp;
        } 
        else if(root -> R== nullptr) {
            Node* tmp = root-> L;
            delete root;
            return tmp;
        } 
        Node * tmp = minNode(root -> R);
        root ->key = tmp -> key;
        root -> data = tmp -> data;
        root -> R = deleteNode(root -> R, tmp -> key);
    }
    return root;
}

bool isFull(Node* root){
    if(root == nullptr) return 1;
    if(root -> L == nullptr && root -> R == nullptr) return 1;
    if(root -> L != nullptr && root -> R != nullptr) return isFull(root -> L) && isFull(root -> R);
    return 0;
}

int main(){
    string filename = "data.csv";

    vector<Match> ds = readFile(filename);

    if (ds.empty()) {
        cout << "Danh sach trong!" << endl;
        return 0;
    }

    // for (int i = 0; i < ds[0].homePlayers.size(); i++) {
    //     cout << "  + " << ds[1].homePlayers[i] << endl;
    // }

    // for (int i = 0; i < ds.size(); i++) {
    //     cout << "Tran " << i + 1 << ": " 
    //          << ds[i].hometeam << " vs " << ds[i].VisitorTeam ;
    // }


    Node* root = createBST(ds);
    vector<string> ketQua;
    NLR(root, ketQua);

    cout << "Duyet NLR: ";
    for (string s : ketQua) {
        cout << s << " -> ";
    }

    string keyXoa = "Premier League"; // Ví dụ giải đấu cần xóa
    cout << "\nDang xoa nut: " << keyXoa << endl;
    root = deleteNode(root, keyXoa);

    // Kiểm tra lại bằng cách duyệt cây
    vector<string> ketQuaMoi;
    LNR(root, ketQuaMoi);
    cout << "Cay sau khi xoa (LNR): ";
    for (string s : ketQuaMoi) cout << s << " -> ";
    //vector<string> inorderTraversal(root);
    // vector<vector<string>> res = levelOrder(root);
    // cout << res.size();
    // for(int i = 0; i < res.size();i++){
    //     for(auto ans : res[i]){
    //         cout<< ans << '\n';
    //     }
    // }
    return 0;
}

