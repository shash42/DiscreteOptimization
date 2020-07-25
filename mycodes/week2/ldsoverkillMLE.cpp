#include<bits/stdc++.h>
#define ld long double
#define pb push_back

using namespace std;

const int N = 1e4+5;
const ld pr = 1e-14;
int n, k;
struct item{
    int v, w, idx;
    ld g;
} obj[N];
struct gsrt{
    bool operator()(const item &a, const item &b){
        if(abs(a.g-b.g)<pr){
            return a.w < b.w;
        }
        return a.g > b.g;
    }
};
struct Relax{
    ld val = 0, wtlst = 0;
    int lstidx = 0;
};
struct Node{
    Relax R;
    vector<bool> decvars;
    int cv = 0, cw = 0, idx = 0;
} best;

void getRelax(int st, Node &curr, ld excesswt){
    for(int i = st; i <= n+1; i++){
        if(i==n+1){
            curr.R.lstidx = n+1;
            curr.R.wtlst = 0;
            break;
        }
        if(obj[i].w < excesswt){
            excesswt -= obj[i].w;
            curr.R.val += obj[i].v;
        }
        else{
            curr.R.val += excesswt * obj[i].g;;
            curr.R.lstidx = i;
            curr.R.wtlst = excesswt;
            excesswt = 0;
            break;
        }
    }
}
Node getLeft(Node u){
    Node lc = u;
    lc.idx++;
    lc.decvars.pb(true);
    lc.cv += obj[lc.idx].v; lc.cw += obj[lc.idx].w;

    //add change in relaxation
    //currently as LDS-Greedy = Relaxation: none.
    
    return lc;
}
Node getRight(Node u){
    Node rc = u;
    rc.idx++;
    rc.decvars.pb(false);
    //Relaxation Computation
    ld excesswt;
    int distribst = rc.R.lstidx + 1;
    if(rc.R.lstidx == rc.idx){
        rc.R.val -= rc.R.wtlst*obj[rc.R.lstidx].g; //remove this weight
        excesswt = rc.R.wtlst;
    }
    else{
        //remove current node from relaxation computation
        rc.R.val -= obj[rc.idx].v;
        excesswt = obj[rc.idx].w;
        //now check if current lstidx is sufficient or not
        if(obj[rc.R.lstidx].w - rc.R.wtlst > excesswt){ //if current last is enough
            rc.R.val += excesswt * obj[rc.R.lstidx].g;
            rc.R.wtlst += excesswt;
            excesswt = 0;
            distribst = n+2; //no more objects required for relaxation computation
        }
        else{
            rc.R.val += (obj[rc.R.lstidx].w - rc.R.wtlst) * obj[rc.R.lstidx].g;
            excesswt -= obj[rc.R.lstidx].w - rc.R.wtlst;  
            rc.R.wtlst = 0; //temporary and useless 
        }
    }
    getRelax(distribst, rc, excesswt);
    return rc;
}
bool eligible(Node &curr){
    return !(curr.cw > k + pr|| curr.R.val < best.cv + pr);
}
void goLeft(vector<Node> &toLeft, vector<Node> &toRight){
    //cerr << "Left: ";
    for(auto u: toLeft){
        //for(auto bit: u.decvars) cerr << bit;
      //  cerr << "-" << u.R.val << " ";
        //cerr << u.idx << " ";
        Node curr = u;
        while(curr.idx < n){
            if(!eligible(curr)) break; //check eligibility
            toRight.pb(curr);
            curr = getLeft(curr);
        }
        if(curr.idx==n && eligible(curr)){ //right children get evaluated here
            if(curr.cv > best.cv){
                best = curr;
            }
        }
    }
   // cerr << endl;
    toLeft.clear();
}
void goRight(vector<Node> &toLeft, vector<Node> &toRight){
    //cerr << "Right: ";
    for(auto u: toRight){
        //for(auto bit: u.decvars) cerr << bit;
      //  cerr << "-" << u.R.val << " ";
      //  cerr << u.idx << " ";
        if(u.idx < n){
            Node rc = getRight(u);
            if(!eligible(rc)) continue; //check eligibility
            toLeft.pb(rc);
        }
        if(u.idx==n && eligible(u)){ //left children get evaluated here
            if(u.cv > best.cv){
                best = u;
            }
        }
    }
    //cerr << endl;
    toRight.clear();
}
void LDS(){
    vector<Node> toLeft, toRight;
    Node init;
    init.decvars.pb(false); //indexing-correction (we use 1-indexing)
    getRelax(1, init, k);
    toLeft.pb(init);
    goLeft(toLeft, toRight);
    for(int i = 1; i <= n; i++){
        cerr << "wave: " << i << " " << toRight.size() << " ";
        goRight(toLeft, toRight);
        cerr << toLeft.size() << endl;
        goLeft(toLeft, toRight);
        cerr << endl;
    }
}
signed main(){
    cin >> n >> k;
    for(int i = 1; i <= n; i++){
        cin >> obj[i].v >> obj[i].w;
        obj[i].idx = i;
        obj[i].g = ((double) obj[i].v) / obj[i].w;
    }
    sort(obj+1, obj+n+1, gsrt());
    for(int i = 1; i <= n; i++){
      //  cerr << obj[i].v << " " << obj[i].w << " " << obj[i].g << endl;
    }
    LDS();
    vector<bool> ans(n+1);
    for(int i = 1; i <= n; i++){
        int idx = obj[i].idx;
        ans[idx] = best.decvars[i];
    }
    cout << best.cv << " " << 1 << endl;
    for(int i = 1; i <= n; i++){
        cout << ans[i] << " ";
    }
}