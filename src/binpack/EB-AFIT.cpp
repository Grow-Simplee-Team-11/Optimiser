//**********************
// INCLUDED HEADER FILES
//**********************

#include <time.h>

#include "../../include/binpack/EB_AFIT.hpp"
#include "../../include/datastructures.hpp"

using namespace std;

void execiterations(vector<item>&);
void listcanditlayers(vector<item>&);
int complayerlist(const void* i, const void* j);
int packlayer(vector<item>&);
int findlayer(vector<item>&, short int thickness);
void findbox(vector<item>& cluster, short int hmx, short int hy, short int hmy, short int hz, short int hmz);
void analyzebox(short int hmx, short int hy, short int hmy, short int hz, short int hmz, short int dim1, short int dim2, short int dim3);
void findsmallestz(vector<item>&);
void checkfound(void);
void volumecheck(vector<item>& cluster);
void report(vector<item>& cluster);

struct layerlist {
    long int layereval;
    short int layerdim;
};

struct scrappad {
    struct scrappad *pre, *pos;
    short int cumx, cumz;

    scrappad(scrappad* pre_ = NULL, scrappad* pos_ = NULL) {
        pre = pre_;
        pos = pos_;
    }
};

char packing;
char layerdone;
char evened;
char variant_;
char bestvariant;
char packingbest;
char hundredpercent;
char unpacked;
char quit;

double boxx, boxy, boxz, boxi;
double bboxx, bboxy, bboxz, bboxi;
double cboxx, cboxy, cboxz, cboxi;
double bfx, bfy, bfz;
double bbfx, bbfy, bbfz;
double xx, yy, zz;
double px, py, pz;

double packedvolume;
double bestvolume;
double totalvolume;
double totalboxvol;
double temp;
double percentageused;
double percentagepackedbox;
double elapsedtime;

short int x;
short int n;
short int layerlistlen;
short int layerinlayer;
short int prelayer;
short int lilz;
short int itenum;
short int hour;
short int min;
short int sec;
short int layersindex;
short int remainpx, remainpy, remainpz;
short int packedy;
short int prepackedy;
short int layerthickness;
short int itelayer;
short int preremainpy;
short int bestite;
short int packednumbox;
short int bestpackednum;

layerlist layers[1000];
int boxStatus[1000];
scrappad *scrapfirst, *scrapmemb, *smallestz, *trash;
time_t start, finish;

EB_AFIT::EB_AFIT() {
}

void sortCluster(vector<item>& cluster) {
    sort(cluster.begin(), cluster.end(), [](const item& a, const item& b) {
        return a.orig_rank < b.orig_rank;
    });
}

double EB_AFIT::getInversionCount(vector<item>& cluster) {
    sortCluster(cluster);
    int num_cnt = 0;
    for (int i = 0; i < cluster.size(); i++) {
        for (int j = i + 1; j < cluster.size(); j++) {
            if (cluster[i].position.y < cluster[j].position.y) {
                if (cluster[i].position.x < cluster[j].position.x + cluster[j].size.length ||
                    cluster[i].position.x + cluster[i].size.length > cluster[j].position.x) {
                    if (cluster[i].position.z < cluster[j].position.z + cluster[j].size.height ||
                        cluster[i].position.z + cluster[i].size.height > cluster[j].position.z) {
                        // cout << "Inversion detected" << endl;
                        // cout << i << " " << j << endl;
                        // // cluster[i].print();
                        // cout << cluster[i].position.x << " " << cluster[i].position.y << " " << cluster[i].position.z << endl;
                        // cout << "and" << endl;
                        // cout << cluster[j].position.x << " " << cluster[j].position.y << " " << cluster[j].position.z << endl;
                        // cluster[j].print();

                        num_cnt++;
                    }
                }
            }
        }
    }
    int n = cluster.size();
    double ans = 1 - (double)num_cnt / ((n * (n - 1)) / 2);
    cout << num_cnt << endl;
    return ans;
}

void EB_AFIT::BinPack(vector<item>& cluster, Bin b) {
    time(&start);
    n = cluster.size();
    totalboxvol = 0;
    for (auto& it : cluster) {
        it.volume = it.size.length * it.size.width * it.size.height;
        totalboxvol += it.volume;
    }
    temp = 1;
    totalvolume = temp * b.size.length * b.size.width * b.size.height;
    scrapfirst = new scrappad;
    for (int i = 0; i < n; i++) {
        boxStatus[i] = 0;
    }
    if (scrapfirst == NULL) {
        printf("Insufficient memory available\n");
        exit(1);
    }
    bestvolume = 0;
    packingbest = 0;
    hundredpercent = 0;
    itenum = 0;
    px = b.size.length;
    py = b.size.width;
    pz = b.size.height;
    for (int i = 0; i < cluster.size(); i++) {
        cluster[i].orig_rank = i;
    }
    execiterations(cluster);
    time(&finish);
    report(cluster);
    this->packed_items = cluster;
    return;
}

float EB_AFIT::CalculateCost() {
    cost = 100 - percentagepackedbox;
    return cost;
}

int complayerlist(const void* i, const void* j) {
    return *(long int*)i - *(long int*)j;
}

void listcanditlayers(vector<item>& cluster) {
    bool same;
    short int exdim, dimdif, dimen2, dimen3, y, z, k;
    long int layereval;

    layerlistlen = 0;

    for (x = 0; x < n; x++) {
        exdim = cluster[x].size.height;
        dimen2 = cluster[x].size.width;
        dimen3 = cluster[x].size.length;
        if ((exdim > py) || (((dimen2 > px) || (dimen3 > pz)) && ((dimen3 > px) || (dimen2 > pz)))) continue;
        same = 0;

        for (k = 1; k <= layerlistlen; k++) {
            if (exdim == layers[k].layerdim) {
                same = 1;
                continue;
            }
        }
        if (same) continue;
        layereval = 0;
        for (z = 0; z < n; z++) {
            if (!(x == z)) {
                dimdif = abs(exdim - cluster[z].size.height);
                layereval = layereval + dimdif;
            }
        }
        layers[++layerlistlen].layereval = layereval;
        layers[layerlistlen].layerdim = exdim;
    }
    return;
}

void execiterations(vector<item>& cluster) {
    listcanditlayers(cluster);
    layers[0].layereval = -1;
    qsort(layers, layerlistlen + 1, sizeof(struct layerlist), complayerlist);

    for (layersindex = 1; (layersindex <= layerlistlen); layersindex++) {
        ++itenum;
        packedvolume = 0.0;
        packedy = 0;
        packing = 1;
        layerthickness = layers[layersindex].layerdim;
        itelayer = layersindex;
        remainpy = py;
        remainpz = pz;
        packednumbox = 0;
        for (x = 0; x < n; x++) {
            boxStatus[x] = 0;
        }

        // BEGIN DO-WHILE
        do {
            layerinlayer = 0;
            layerdone = 0;
            // Bookmark 1 here
            if (packlayer(cluster)) {
                exit(1);
            }
            packedy = packedy + layerthickness;
            remainpy = py - packedy;
            if (layerinlayer) {
                prepackedy = packedy;
                preremainpy = remainpy;
                remainpy = layerthickness - prelayer;
                packedy = packedy - layerthickness + prelayer;
                remainpz = lilz;
                layerthickness = layerinlayer;
                layerdone = 0;
                if (packlayer(cluster)) {
                    exit(1);
                }
                packedy = prepackedy;
                remainpy = preremainpy;
                remainpz = pz;
            }
            findlayer(cluster, remainpy);
        } while (packing);
        // END DO-WHILE

        if ((packedvolume > bestvolume)) {
            bestvolume = packedvolume;
            bestvariant = variant_;
            bestite = itelayer;
            bestpackednum = packednumbox;
        }

        if (hundredpercent) break;
        percentageused = bestvolume * 100 / totalvolume;
    }
}

int packlayer(vector<item>& cluster) {
    short int lenx, lenz, lpz;

    if (!layerthickness) {
        packing = 0;
        return 0;
    }

    scrapfirst->cumx = px;
    scrapfirst->cumz = 0;

    for (;;) {
        // Bookmark 2
        findsmallestz(cluster);

        if (!(smallestz->pre) && !(smallestz->pos)) {
            //*** SITUATION-1: NO BOXES ON THE RIGHT AND LEFT SIDES ***

            lenx = smallestz->cumx;
            lpz = remainpz - (smallestz->cumz);
            //   Bookmark 3
            findbox(cluster, lenx, layerthickness, remainpy, lpz, lpz);
            checkfound();

            if (layerdone) break;
            if (evened) continue;

            cluster[cboxi].position.x = 0;
            cluster[cboxi].position.y = packedy;
            cluster[cboxi].position.z = (*smallestz).cumz;
            if (cboxx == (*smallestz).cumx) {
                (*smallestz).cumz = (*smallestz).cumz + cboxz;
            } else {
                (*smallestz).pos = new scrappad;
                if ((*smallestz).pos == NULL) {
                    printf("Insufficient memory available\n");
                    return 1;
                }
                (*((*smallestz).pos)).pos = NULL;
                (*((*smallestz).pos)).pre = smallestz;
                (*((*smallestz).pos)).cumx = (*smallestz).cumx;
                (*((*smallestz).pos)).cumz = (*smallestz).cumz;
                (*smallestz).cumx = cboxx;
                (*smallestz).cumz = (*smallestz).cumz + cboxz;
            }
            volumecheck(cluster);
        } else if (!(*smallestz).pre) {
            //*** SITUATION-2: NO BOXES ON THE LEFT SIDE ***

            lenx = (*smallestz).cumx;
            lenz = (*((*smallestz).pos)).cumz - (*smallestz).cumz;
            lpz = remainpz - (*smallestz).cumz;
            findbox(cluster, lenx, layerthickness, remainpy, lenz, lpz);
            checkfound();

            if (layerdone) break;
            if (evened) continue;

            cluster[cboxi].position.y = packedy;
            cluster[cboxi].position.z = (*smallestz).cumz;
            if (cboxx == (*smallestz).cumx) {
                cluster[cboxi].position.x = 0;
                if ((*smallestz).cumz + cboxz == (*((*smallestz).pos)).cumz) {
                    (*smallestz).cumz = (*((*smallestz).pos)).cumz;
                    (*smallestz).cumx = (*((*smallestz).pos)).cumx;
                    trash = (*smallestz).pos;
                    (*smallestz).pos = (*((*smallestz).pos)).pos;
                    if ((*smallestz).pos) {
                        (*((*smallestz).pos)).pre = smallestz;
                    }
                    // delete trash;
                } else {
                    (*smallestz).cumz = (*smallestz).cumz + cboxz;
                }
            } else {
                cluster[cboxi].position.x = (*smallestz).cumx - cboxx;
                if ((*smallestz).cumz + cboxz == (*((*smallestz).pos)).cumz) {
                    (*smallestz).cumx = (*smallestz).cumx - cboxx;
                } else {
                    (*((*smallestz).pos)).pre = new scrappad;
                    if ((*((*smallestz).pos)).pre == NULL) {
                        printf("Insufficient memory available\n");
                        return 1;
                    }
                    (*((*((*smallestz).pos)).pre)).pos = (*smallestz).pos;
                    (*((*((*smallestz).pos)).pre)).pre = smallestz;
                    (*smallestz).pos = (*((*smallestz).pos)).pre;
                    (*((*smallestz).pos)).cumx = (*smallestz).cumx;
                    (*smallestz).cumx = (*smallestz).cumx - cboxx;
                    (*((*smallestz).pos)).cumz = (*smallestz).cumz + cboxz;
                }
            }
            volumecheck(cluster);
        } else if (!(*smallestz).pos) {
            //*** SITUATION-3: NO BOXES ON THE RIGHT SIDE ***

            lenx = (*smallestz).cumx - (*((*smallestz).pre)).cumx;
            lenz = (*((*smallestz).pre)).cumz - (*smallestz).cumz;
            lpz = remainpz - (*smallestz).cumz;
            findbox(cluster, lenx, layerthickness, remainpy, lenz, lpz);
            checkfound();

            if (layerdone) break;
            if (evened) continue;

            cluster[cboxi].position.y = packedy;
            cluster[cboxi].position.z = (*smallestz).cumz;
            cluster[cboxi].position.x = (*((*smallestz).pre)).cumx;

            if (cboxx == (*smallestz).cumx - (*((*smallestz).pre)).cumx) {
                if ((*smallestz).cumz + cboxz == (*((*smallestz).pre)).cumz) {
                    (*((*smallestz).pre)).cumx = (*smallestz).cumx;
                    (*((*smallestz).pre)).pos = NULL;
                    // delete smallestz;
                } else {
                    (*smallestz).cumz = (*smallestz).cumz + cboxz;
                }
            } else {
                if ((*smallestz).cumz + cboxz == (*((*smallestz).pre)).cumz) {
                    (*((*smallestz).pre)).cumx = (*((*smallestz).pre)).cumx + cboxx;
                } else {
                    (*((*smallestz).pre)).pos = new scrappad;
                    if ((*((*smallestz).pre)).pos == NULL) {
                        printf("Insufficient memory available\n");
                        return 1;
                    }
                    (*((*((*smallestz).pre)).pos)).pre = (*smallestz).pre;
                    (*((*((*smallestz).pre)).pos)).pos = smallestz;
                    (*smallestz).pre = (*((*smallestz).pre)).pos;
                    (*((*smallestz).pre)).cumx = (*((*((*smallestz).pre)).pre)).cumx + cboxx;
                    (*((*smallestz).pre)).cumz = (*smallestz).cumz + cboxz;
                }
            }
            volumecheck(cluster);
        } else if ((*((*smallestz).pre)).cumz == (*((*smallestz).pos)).cumz) {
            //*** SITUATION-4: THERE ARE BOXES ON BOTH OF THE SIDES ***

            //*** SUBSITUATION-4A: SIDES ARE EQUAL TO EACH OTHER ***

            lenx = (*smallestz).cumx - (*((*smallestz).pre)).cumx;
            lenz = (*((*smallestz).pre)).cumz - (*smallestz).cumz;
            lpz = remainpz - (*smallestz).cumz;

            findbox(cluster, lenx, layerthickness, remainpy, lenz, lpz);
            checkfound();

            if (layerdone) break;
            if (evened) continue;

            cluster[cboxi].position.y = packedy;
            cluster[cboxi].position.z = (*smallestz).cumz;
            if (cboxx == (*smallestz).cumx - (*((*smallestz).pre)).cumx) {
                cluster[cboxi].position.x = (*((*smallestz).pre)).cumx;
                if ((*smallestz).cumz + cboxz == (*((*smallestz).pos)).cumz) {
                    (*((*smallestz).pre)).cumx = (*((*smallestz).pos)).cumx;
                    if ((*((*smallestz).pos)).pos) {
                        (*((*smallestz).pre)).pos = (*((*smallestz).pos)).pos;
                        (*((*((*smallestz).pos)).pos)).pre = (*smallestz).pre;
                        // delete smallestz;
                    } else {
                        (*((*smallestz).pre)).pos = NULL;
                        // delete smallestz;
                    }
                } else {
                    (*smallestz).cumz = (*smallestz).cumz + cboxz;
                }
            } else if ((*((*smallestz).pre)).cumx < px - (*smallestz).cumx) {
                if ((*smallestz).cumz + cboxz == (*((*smallestz).pre)).cumz) {
                    (*smallestz).cumx = (*smallestz).cumx - cboxx;
                    cluster[cboxi].position.x = (*smallestz).cumx - cboxx;
                } else {
                    cluster[cboxi].position.x = (*((*smallestz).pre)).cumx;
                    (*((*smallestz).pre)).pos = new scrappad;
                    if ((*((*smallestz).pre)).pos == NULL) {
                        printf("Insufficient memory available\n");
                        return 1;
                    }
                    (*((*((*smallestz).pre)).pos)).pre = (*smallestz).pre;
                    (*((*((*smallestz).pre)).pos)).pos = smallestz;
                    (*smallestz).pre = (*((*smallestz).pre)).pos;
                    (*((*smallestz).pre)).cumx = (*((*((*smallestz).pre)).pre)).cumx + cboxx;
                    (*((*smallestz).pre)).cumz = (*smallestz).cumz + cboxz;
                }
            } else {
                if ((*smallestz).cumz + cboxz == (*((*smallestz).pre)).cumz) {
                    (*((*smallestz).pre)).cumx = (*((*smallestz).pre)).cumx + cboxx;
                    cluster[cboxi].position.x = (*((*smallestz).pre)).cumx;
                } else {
                    cluster[cboxi].position.x = (*smallestz).cumx - cboxx;
                    (*((*smallestz).pos)).pre = new scrappad;
                    if ((*((*smallestz).pos)).pre == NULL) {
                        printf("Insufficient memory available\n");
                        return 1;
                    }
                    (*((*((*smallestz).pos)).pre)).pos = (*smallestz).pos;
                    (*((*((*smallestz).pos)).pre)).pre = smallestz;
                    (*smallestz).pos = (*((*smallestz).pos)).pre;
                    (*((*smallestz).pos)).cumx = (*smallestz).cumx;
                    (*((*smallestz).pos)).cumz = (*smallestz).cumz + cboxz;
                    (*smallestz).cumx = (*smallestz).cumx - cboxx;
                }
            }
            volumecheck(cluster);
        } else {
            //*** SUBSITUATION-4B: SIDES ARE NOT EQUAL TO EACH OTHER ***

            lenx = (*smallestz).cumx - (*((*smallestz).pre)).cumx;
            lenz = (*((*smallestz).pre)).cumz - (*smallestz).cumz;
            lpz = remainpz - (*smallestz).cumz;
            findbox(cluster, lenx, layerthickness, remainpy, lenz, lpz);
            checkfound();

            if (layerdone) break;
            if (evened) continue;

            cluster[cboxi].position.y = packedy;
            cluster[cboxi].position.z = (*smallestz).cumz;
            cluster[cboxi].position.x = (*((*smallestz).pre)).cumx;
            if (cboxx == (*smallestz).cumx - (*((*smallestz).pre)).cumx) {
                if ((*smallestz).cumz + cboxz == (*((*smallestz).pre)).cumz) {
                    (*((*smallestz).pre)).cumx = (*smallestz).cumx;
                    (*((*smallestz).pre)).pos = (*smallestz).pos;
                    (*((*smallestz).pos)).pre = (*smallestz).pre;
                    // delete smallestz;
                } else {
                    (*smallestz).cumz = (*smallestz).cumz + cboxz;
                }
            } else {
                if ((*smallestz).cumz + cboxz == (*((*smallestz).pre)).cumz) {
                    (*((*smallestz).pre)).cumx = (*((*smallestz).pre)).cumx + cboxx;
                } else if ((*smallestz).cumz + cboxz == (*((*smallestz).pos)).cumz) {
                    cluster[cboxi].position.x = (*smallestz).cumx - cboxx;
                    (*smallestz).cumx = (*smallestz).cumx - cboxx;
                } else {
                    (*((*smallestz).pre)).pos = new scrappad;
                    if ((*((*smallestz).pre)).pos == NULL) {
                        printf("Insufficient memory available\n");
                        return 1;
                    }
                    (*((*((*smallestz).pre)).pos)).pre = (*smallestz).pre;
                    (*((*((*smallestz).pre)).pos)).pos = smallestz;
                    (*smallestz).pre = (*((*smallestz).pre)).pos;
                    (*((*smallestz).pre)).cumx = (*((*((*smallestz).pre)).pre)).cumx + cboxx;
                    (*((*smallestz).pre)).cumz = (*smallestz).cumz + cboxz;
                }
            }
            volumecheck(cluster);
        }
    }
    return 0;
}

int findlayer(vector<item>& cluster, short int thickness) {
    short int exdim, dimdif, dimen2, dimen3, y, z;
    long int layereval, eval;
    layerthickness = 0;
    eval = 1000000;
    for (x = 0; x < n; x++) {
        if (boxStatus[x]) continue;
        exdim = cluster[x].size.height;
        dimen2 = cluster[x].size.width;
        dimen3 = cluster[x].size.length;
        layereval = 0;
        if ((exdim <= thickness) && (((dimen2 <= px) && (dimen3 <= pz)) || ((dimen3 <= px) && (dimen2 <= pz)))) {
            for (z = 0; z < n; z++) {
                if (!(x == z) && !(boxStatus[int(z)])) {
                    dimdif = abs(exdim - cluster[z].size.height);
                    // if ( abs(exdim - boxlist[z].dim2) < dimdif )
                    // {
                    //   dimdif = abs(exdim - boxlist[z].dim2);
                    // }
                    // if ( abs(exdim - boxlist[z].dim3) < dimdif )
                    // {
                    //   dimdif = abs(exdim - boxlist[z].dim3);
                    // }
                    layereval = layereval + dimdif;
                }
            }
            if (layereval < eval) {
                eval = layereval;
                layerthickness = exdim;
            }
        }
    }
    if (layerthickness == 0 || layerthickness > remainpy) packing = 0;
    return 0;
}

void findbox(vector<item>& cluster, short int hmx, short int hy, short int hmy, short int hz, short int hmz) {
    short int y;
    bfx = 32767;
    bfy = 32767;
    bfz = 32767;
    bbfx = 32767;
    bbfy = 32767;
    bbfz = 32767;
    boxi = -1;
    bboxi = -1;
    for (x = 0; x < n; x++) {
        if (boxStatus[x]) continue;
        if (x > n) return;
        analyzebox(hmx, hy, hmy, hz, hmz, cluster[x].size.width, cluster[x].size.height, cluster[x].size.length);
        analyzebox(hmx, hy, hmy, hz, hmz, cluster[x].size.length, cluster[x].size.height, cluster[x].size.width);
    }
}

//**********************************************************************
// ANALYZES EACH UNPACKED BOX TO FIND THE BEST FITTING ONE TO
// THE EMPTY SPACE GIVEN
//**********************************************************************

void analyzebox(short int hmx, short int hy, short int hmy, short int hz, short int hmz, short int dim1, short int dim2, short int dim3) {
    if (dim1 <= hmx && dim2 <= hmy && dim3 <= hmz) {
        if (dim2 <= hy) {
            if (hy - dim2 < bfy) {
                boxx = dim1;
                boxy = dim2;
                boxz = dim3;
                bfx = hmx - dim1;
                bfy = hy - dim2;
                bfz = abs(hz - dim3);
                boxi = x;
            } else if (hy - dim2 == bfy && hmx - dim1 < bfx) {
                boxx = dim1;
                boxy = dim2;
                boxz = dim3;
                bfx = hmx - dim1;
                bfy = hy - dim2;
                bfz = abs(hz - dim3);
                boxi = x;
            } else if (hy - dim2 == bfy && hmx - dim1 == bfx && abs(hz - dim3) < bfz) {
                boxx = dim1;
                boxy = dim2;
                boxz = dim3;
                bfx = hmx - dim1;
                bfy = hy - dim2;
                bfz = abs(hz - dim3);
                boxi = x;
            }
        } else {
            if (dim2 - hy < bbfy) {
                bboxx = dim1;
                bboxy = dim2;
                bboxz = dim3;
                bbfx = hmx - dim1;
                bbfy = dim2 - hy;
                bbfz = abs(hz - dim3);
                bboxi = x;
            } else if (dim2 - hy == bbfy && hmx - dim1 < bbfx) {
                bboxx = dim1;
                bboxy = dim2;
                bboxz = dim3;
                bbfx = hmx - dim1;
                bbfy = dim2 - hy;
                bbfz = abs(hz - dim3);
                bboxi = x;
            } else if (dim2 - hy == bbfy && hmx - dim1 == bbfx && abs(hz - dim3) < bbfz) {
                bboxx = dim1;
                bboxy = dim2;
                bboxz = dim3;
                bbfx = hmx - dim1;
                bbfy = dim2 - hy;
                bbfz = abs(hz - dim3);
                bboxi = x;
            }
        }
    }
}

//********************************************************
// FINDS THE FIRST TO BE PACKED GAP IN THE LAYER EDGE
//********************************************************
void findsmallestz(vector<item>&) {
    scrapmemb = scrapfirst;
    smallestz = scrapmemb;
    while (!(scrapmemb->pos == NULL)) {
        if ((scrapmemb->pos)->cumz < smallestz->cumz) {
            smallestz = scrapmemb->pos;
        }
        scrapmemb = scrapmemb->pos;
    }
    return;
}

//************************************************************
// AFTER FINDING EACH BOX, THE CANDIDATE BOXES AND THE
// CONDITION OF THE LAYER ARE EXAMINED
//************************************************************

void checkfound(void) {
    evened = 0;
    if (boxi != -1) {
        cboxi = boxi;
        cboxx = boxx;
        cboxy = boxy;
        cboxz = boxz;
    } else {
        if ((bboxi >= 0) && (layerinlayer || (!(smallestz->pre) && !(smallestz->pos)))) {
            if (!layerinlayer) {
                prelayer = layerthickness;
                lilz = smallestz->cumz;
            }
            cboxi = bboxi;
            cboxx = bboxx;
            cboxy = bboxy;
            cboxz = bboxz;
            layerinlayer = layerinlayer + bboxy - layerthickness;
            layerthickness = bboxy;
        } else {
            if (!(*smallestz).pre && !(*smallestz).pos) {
                layerdone = 1;
            } else {
                evened = 1;
                if (!(*smallestz).pre) {
                    trash = (*smallestz).pos;
                    (*smallestz).cumx = (*((*smallestz).pos)).cumx;
                    (*smallestz).cumz = (*((*smallestz).pos)).cumz;
                    (*smallestz).pos = (*((*smallestz).pos)).pos;
                    if ((*smallestz).pos) {
                        (*((*smallestz).pos)).pre = smallestz;
                    }
                    // delete trash;
                } else if (!(*smallestz).pos) {
                    (*((*smallestz).pre)).pos = NULL;
                    (*((*smallestz).pre)).cumx = (*smallestz).cumx;
                    // delete smallestz;
                } else {
                    if ((*((*smallestz).pre)).cumz == (*((*smallestz).pos)).cumz) {
                        (*((*smallestz).pre)).pos = (*((*smallestz).pos)).pos;
                        if ((*((*smallestz).pos)).pos) {
                            (*((*((*smallestz).pos)).pos)).pre = (*smallestz).pre;
                        }
                        (*((*smallestz).pre)).cumx = (*((*smallestz).pos)).cumx;
                        // delete (*smallestz).pos;
                        // delete smallestz;
                    } else {
                        (*((*smallestz).pre)).pos = (*smallestz).pos;
                        (*((*smallestz).pos)).pre = (*smallestz).pre;
                        if ((*((*smallestz).pre)).cumz < (*((*smallestz).pos)).cumz) {
                            (*((*smallestz).pre)).cumx = (*smallestz).cumx;
                        }
                        // delete smallestz;
                    }
                }
            }
        }
    }
    return;
}

//********************************************************************
// AFTER PACKING OF EACH BOX, 100% PACKING CONDITION IS CHECKED
//********************************************************************

void volumecheck(vector<item>& cluster) {
    boxStatus[int(cboxi)] = 1;
    cluster[cboxi].size.length = cboxx;
    cluster[cboxi].size.height = cboxy;
    cluster[cboxi].size.width = cboxz;
    packedvolume = packedvolume + cluster[cboxi].volume;
    packednumbox++;
    if (packedvolume == totalvolume || packedvolume == totalboxvol) {
        packing = 0;
        hundredpercent = 1;
    }
    return;
}

//*******************************************************************
// USING THE PARAMETERS FOUND, PACKS THE BEST SOLUTION FOUND
// AND REPORS TO THE CONSOLE AND TO A TEXT FILE
//*******************************************************************

void report(vector<item>& cluster) {
    packingbest = 1;

    percentagepackedbox = bestvolume * 100 / totalboxvol;
    percentageused = bestvolume * 100 / totalvolume;
    elapsedtime = difftime(finish, start);

    listcanditlayers(cluster);
    layers[0].layereval = -1;
    qsort(layers, layerlistlen + 1, sizeof(struct layerlist), complayerlist);
    packedvolume = 0.0;
    packedy = 0;
    packing = 1;
    layerthickness = layers[bestite].layerdim;
    remainpy = py;
    remainpz = pz;

    for (x = 0; x < n; x++) {
        boxStatus[int(x)] = 0;
    }

    do {
        layerinlayer = 0;
        layerdone = 0;
        packlayer(cluster);
        packedy = packedy + layerthickness;
        remainpy = py - packedy;
        if (layerinlayer) {
            prepackedy = packedy;
            preremainpy = remainpy;
            remainpy = layerthickness - prelayer;
            packedy = packedy - layerthickness + prelayer;
            remainpz = lilz;
            layerthickness = layerinlayer;
            layerdone = 0;
            packlayer(cluster);
            packedy = prepackedy;
            remainpy = preremainpy;
            remainpz = pz;
        }
        if (!quit) {
            findlayer(cluster, remainpy);
        }
    } while (packing && !quit);

    unpacked = 1;
    unpacked = 0;
    for (x = 0; x < n; x++) {
        if (boxStatus[x]) {
            printf("%d %f %f %f %f %f %f %f %f %f\n", n, cluster[x].size.length, cluster[x].size.width, cluster[x].size.height, cluster[x].position.x, cluster[x].position.y, cluster[x].position.z, cluster[x].size.length, cluster[x].size.width, cluster[x].size.height);
        }
    }

    printf("TOTAL NUMBER OF BOXES              : %d\n", n);
    printf("PACKED NUMBER OF BOXES             : %d\n", bestpackednum);
    printf("TOTAL VOLUME OF ALL BOXES          : %.0f\n", totalboxvol);
    printf("PALLET VOLUME                      : %.0f\n", totalvolume);
    printf("BEST SOLUTION'S VOLUME UTILIZATION : %.0f OUT OF %.0f\n", bestvolume, totalvolume);
    printf("PERCENTAGE OF PALLET VOLUME USED   : %.6f %%\n", percentageused);
    printf("PERCENTAGE OF PACKEDBOXES (VOLUME) : %.6f%%\n", percentagepackedbox);
}
int get_rand(int a,int b){
    return a + rand()%(b - a);
}
// int main() {
//     EB_AFIT eba;
//     //     104, 96, 84
//     // 1. 70, 104, 24, 4
//     // 2. 14, 104, 48, 2
//     // 3. 40, 52, 36, 3
//     srand(time(NULL));
//     int CLUSTER_SIZE = 25;
//     vector<int> min_dimensions(3),max_dimensions(3),bin_dimensions(3);
//     min_dimensions = {3,3,3};
//     max_dimensions = {40,20,40};
//     bin_dimensions = {80,100,80};

//     vector<item> cluster(CLUSTER_SIZE);
//     for(int i = 0;i < CLUSTER_SIZE;i++){
//         cluster[i].size.length = get_rand(min_dimensions[0],max_dimensions[0]);
//         cluster[i].size.width = get_rand(min_dimensions[1],max_dimensions[1]);
//         cluster[i].size.height = get_rand(min_dimensions[2],max_dimensions[2]);
//     }
//     // cluster[0].size.width = 10;
//     // cluster[0].size.length = 10;
//     // cluster[0].size.height = 10;
//     // cluster[1].size.width = 20;
//     // cluster[1].size.length = 20;
//     // cluster[1].size.height = 20;
//     // cluster[2].size.width = 20;
//     // cluster[2].size.length = 20;
//     // cluster[2].size.height = 30;

//     Bin b(bin_dimensions[0], bin_dimensions[1], bin_dimensions[2]);
//     eba.BinPack(cluster, b);
//     auto res = eba.GetPackaging();
//     eba.CalculateCost();
//     cout << eba.GetPackagingCost() << endl;
//     cout << eba.getInversionCount(cluster) << endl;
//     // for (auto x : cluster) {
//     //     // x.print();
//     //     cout << x.orig_rank << "\n";
//     //     cout << "\n";
//     //     cout << x.position.x << " " << x.position.y << " " << x.position.z << endl;
//     // }

//     return 0;
// }