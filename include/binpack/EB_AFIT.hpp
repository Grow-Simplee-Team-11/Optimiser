#ifndef __EB_AFIT__HEADER__
#define __EB_AFIT__HEADER__
#include "../interface/BinPackInterface.hpp"


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

class Binpacker{
  public:
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

    Binpacker(){

    }
    
    static int complayerlist(const void* i, const void* j) {
        return *(long int*)i - *(long int*)j;
    }
    void operator=(Binpacker& binpacker){
        packing = binpacker.packing;
        layerdone = binpacker.layerdone;
        evened = binpacker.evened;
        variant_ = binpacker.variant_;
        bestvariant = binpacker.bestvariant;
        packingbest = binpacker.packingbest;
        hundredpercent = binpacker.hundredpercent;
        unpacked = binpacker.unpacked;
        quit = binpacker.quit;

        px = binpacker.px; 
        py = binpacker.py;
        pz = binpacker.pz;

        packedvolume = binpacker.packedvolume;
        bestvolume = binpacker.bestvolume;
        totalvolume = binpacker.totalvolume;
        totalboxvol = binpacker.totalboxvol;
        temp = binpacker.temp;
        percentageused = binpacker.percentageused;
        percentagepackedbox = binpacker.percentagepackedbox;
        elapsedtime = binpacker.elapsedtime;

        boxx = binpacker.boxx;
        boxy = binpacker.boxy;
        boxz = binpacker.boxz; 
        boxi = binpacker.boxi;

        bboxx = binpacker.bboxx; 
        bboxy = binpacker.bboxy;
        bboxz = binpacker.bboxz; 
        bboxi = binpacker.bboxi;

        cboxx = binpacker.cboxx; 
        cboxy = binpacker.cboxy;
        cboxz = binpacker.cboxz; 
        cboxi = binpacker.cboxi;

        bfx = binpacker.bfx; 
        bfy = binpacker.bfy;
        bfz = binpacker.bfz; 
        
        bbfx = binpacker.bbfx; 
        bbfy = binpacker.bbfy;
        bbfz = binpacker.bbfz;

        n = binpacker.n;
        layerlistlen = binpacker.layerlistlen;
        layerinlayer = binpacker.layerinlayer;
        prelayer = binpacker.prelayer;
        lilz = binpacker.lilz;
        itenum = binpacker.itenum;
        layersindex = binpacker.layersindex;
        remainpx = binpacker.remainpx;
        remainpy = binpacker.remainpy; 
        remainpz = binpacker.remainpz;
        packedy = binpacker.packedy;
        prepackedy = binpacker.prepackedy;
        layerthickness = binpacker.layerthickness;
        itelayer = binpacker.itelayer;
        preremainpy = binpacker.preremainpy;
        bestite = binpacker.bestite;
        packednumbox = binpacker.packednumbox;
        bestpackednum = binpacker.bestpackednum;

        for(int i=0;i<n;i++){
            boxStatus[i] = 0;
        }

        
        scrapfirst = new scrappad;
        scrapfirst->cumx = (binpacker.scrapfirst)->cumx;
        scrapfirst->cumz = (binpacker.scrapfirst)->cumz;

        scrapmemb = scrapfirst;
        trash = NULL;
        smallestz = NULL;

        binpacker.scrapmemb = binpacker.scrapfirst;

        if(binpacker.scrapmemb != NULL){
            binpacker.scrapmemb = (binpacker.scrapmemb)->pos;
        }

        while(binpacker.scrapmemb!=NULL){
            scrapmemb->pos = new scrappad;
            scrapmemb->pos->cumx = (binpacker.scrapmemb)->cumx;
            scrapmemb->pos->cumz = (binpacker.scrapmemb)->cumz;
            scrapmemb->pos->pre = scrapmemb;
            scrapmemb = scrapmemb->pos;

            binpacker.scrapmemb = (binpacker.scrapmemb)->pos;
        }
    }

    Binpacker(Binpacker& binpacker, int newSize){
        packing = binpacker.packing;
        layerdone = binpacker.layerdone;
        evened = binpacker.evened;
        variant_ = binpacker.variant_;
        bestvariant = binpacker.bestvariant;
        packingbest = binpacker.packingbest;
        hundredpercent = binpacker.hundredpercent;
        unpacked = binpacker.unpacked;
        quit = binpacker.quit;
        
        px = binpacker.px; 
        py = binpacker.py;
        pz = binpacker.pz;

        boxx = binpacker.boxx;
        boxy = binpacker.boxy;
        boxz = binpacker.boxz; 
        boxi = binpacker.boxi;
        bboxx = binpacker.bboxx; 
        bboxy = binpacker.bboxy;
        bboxz = binpacker.bboxz; 
        bboxi = binpacker.bboxi;
        cboxx = binpacker.cboxx; 
        cboxy = binpacker.cboxy;
        cboxz = binpacker.cboxz; 
        cboxi = binpacker.cboxi;
        bfx = binpacker.bfx; 
        bfy = binpacker.bfy;
        bfz = binpacker.bfz; 
        bbfx = binpacker.bbfx; 
        bbfy = binpacker.bbfy;
        bbfz = binpacker.bbfz;

        packedvolume = binpacker.packedvolume;
        bestvolume = binpacker.bestvolume;
        totalvolume = binpacker.totalvolume;
        totalboxvol = binpacker.totalboxvol;
        temp = binpacker.temp;
        percentageused = binpacker.percentageused;
        percentagepackedbox = binpacker.percentagepackedbox;
        elapsedtime = binpacker.elapsedtime;

        n = newSize;
        layerlistlen = binpacker.layerlistlen;
        layerinlayer = binpacker.layerinlayer;
        prelayer = binpacker.prelayer;
        lilz = binpacker.lilz;
        itenum = binpacker.itenum;
        layersindex = binpacker.layersindex;
        remainpx = binpacker.remainpx;
        remainpy = binpacker.remainpy; 
        remainpz = binpacker.remainpz;
        packedy = binpacker.packedy;
        prepackedy = binpacker.prepackedy;
        layerthickness = binpacker.layerthickness;
        itelayer = binpacker.itelayer;
        preremainpy = binpacker.preremainpy;
        bestite = binpacker.bestite;
        packednumbox = binpacker.packednumbox;
        bestpackednum = binpacker.bestpackednum;

        for(int i=0;i<n;i++){
            boxStatus[i] = 0;
        }
        
        scrapfirst = new scrappad;
        scrapfirst->cumx = (binpacker.scrapfirst)->cumx;
        scrapfirst->cumz = (binpacker.scrapfirst)->cumz;

        scrapmemb = scrapfirst;
        trash = NULL;
        smallestz = NULL;

        binpacker.scrapmemb = binpacker.scrapfirst;
        
        if(binpacker.scrapmemb != NULL){
            binpacker.scrapmemb = (binpacker.scrapmemb)->pos;
        }

        while(binpacker.scrapmemb!=NULL){
            scrapmemb->pos = new scrappad;
            scrapmemb->pos->cumx = (binpacker.scrapmemb)->cumx;
            scrapmemb->pos->cumz = (binpacker.scrapmemb)->cumz;
            scrapmemb->pos->pre = scrapmemb;
            scrapmemb = scrapmemb->pos;

            binpacker.scrapmemb = (binpacker.scrapmemb)->pos;
        }
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
        // cout<<"Printing Layers information"<<endl;
        // for(int i=1;i<=layerlistlen;i++){
        //     cout<<layers[i].layerdim<<endl;
        // }
        // cout<<"Ending Layer information"<<endl;
        layers[0].layereval = -1;
        qsort(layers, layerlistlen + 1, sizeof(struct layerlist), complayerlist);

        layersindex = 1;
        ++itenum;
        packedvolume = 0.0;
        packedy = 0;
        packing = 1;
        layerthickness = layers[layersindex].layerdim;
        itelayer = layersindex;
        packednumbox = 0;
        for (x = 0; x < n; x++) {
            boxStatus[x] = 0;
        }

        // cout<<"Done with exceiterations"<<endl;

        // BEGIN DO-WHILE
        do {
            layerinlayer = 0;
            layerdone = 0;
            // Bookmark 1a here
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
        // cout<<"HELLLLLLLLLLLLL"<<endl;
        // for(auto x:cluster){
        //     cout<<x.position.x<<" "<<x.position.y<<" "<<x.position.z<<endl;
        // }

        percentageused = bestvolume * 100 / totalvolume;
    }

    int packlayer(vector<item>& cluster) {
        short int lenx, lenz, lpz;

        if (!layerthickness) {
            packing = 0;
            return 0;
        }

        scrapfirst->cumx = px;
        scrapfirst->cumz = 0;

        // cout<<"Entering packlayer loop"<<endl;
        for (;;) {
            // Bookmark 2
            findsmallestz(cluster);
            // cout<<smallestz->pre<<" "<<smallestz->pos<<endl;

            if (!(smallestz->pre) && !(smallestz->pos)) {
                //*** SITUATION-1: NO BOXES ON THE RIGHT AND LEFT SIDES ***

                lenx = smallestz->cumx;
                lpz = remainpz - (smallestz->cumz);
                // cout<<"In Packlayer"<<endl;
                // cout<<lenx<<" "<<lpz<<" "<<remainpy<<endl;
                // cout<<"Out Packlayer"<<endl;
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

                // cout<<"In Packlayer"<<endl;
                // cout<<lenx<<" "<<" "<<lenz<<" "<<lpz<<" "<<remainpy<<endl;
                // cout<<"Out Packlayer"<<endl;

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
                // cout<<"In Packlayer"<<endl;
                // cout<<lenx<<" "<<" "<<lenz<<" "<<lpz<<" "<<remainpy<<endl;
                // cout<<"Out Packlayer"<<endl;
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
                // cout<<"In Packlayer"<<endl;
                // cout<<lenx<<" "<<lenz<<" "<<lpz<<" "<<remainpy<<endl;
                // cout<<"Out Packlayer"<<endl;
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
                // cout<<"In Packlayer"<<endl;
                // cout<<lenx<<" "<<lpz<<endl;
                // cout<<"Out Packlayer"<<endl;
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
        int num = 0;
        double packedVol = 0.0;
        for(auto x:cluster){
            if(x.position.x!=-1){
                num++;
                packedVol += x.volume;
            }
        }

        percentageused = (packedVol/totalvolume) * 100;
        percentagepackedbox = ((double)num)/((double)n) * 100;


        printf("TOTAL NUMBER OF BOXES              : %d\n", n);
        printf("PACKED NUMBER OF BOXES             : %d\n", num);
        printf("TOTAL VOLUME OF ALL BOXES          : %.0f\n", totalboxvol);
        printf("PALLET VOLUME                      : %.0f\n", totalvolume);
        printf("BEST SOLUTION'S VOLUME UTILIZATION : %.0f OUT OF %.0f\n", packedVol, totalvolume);
        printf("PERCENTAGE OF PALLET VOLUME USED   : %.6f %%\n", percentageused);
        printf("PERCENTAGE OF PACKEDBOXES (VOLUME) : %.6f%%\n", percentagepackedbox);
    }

};

class EB_AFIT : public BinPackInterface {
   public:
    Binpacker binpacker;
    EB_AFIT();
    void BinPack(vector<item>& cluster, Bin b);
    float CalculateCost();
    double getInversionCount(vector<item>& cluster);
};
#endif