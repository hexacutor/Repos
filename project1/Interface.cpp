#pragma once
#include "Interface.h"
#include <time.h>
#include "methods.h"
Interface::Interface(int id, bool write_log, Controller* c) : id(id), write_log(write_log), cc(c)
{
    if (write_log) {
        log_file_name = "logint.txt";
        logger = ofstream(log_file_name, ios::app);
        struct tm* a;
        const time_t timer = time(NULL);
        a = localtime(&timer);
        logger << "\n";
        logger << a->tm_mday << "." << (a->tm_mon + 1) << " " << (a->tm_hour) << ":" << (a->tm_min) << ":" << (a->tm_sec) << " -- " << "NEW SESSION STARTED" << endl;
    }
}

int Interface::run()
{
    cout << "HELP-spisok komand" << endl;
    string main;
    cin >> main;
    for (auto& c : main) c = toupper(c);
    while (main != "EXIT")
    {
        if (main == "HELP")
        {
            string manual;
            int err = cc->help(id, manual);
            if (err != 0)
            {
                show("Bad help file: " + manual);
            }
            else {
                ifstream help(manual);
                if (!help.is_open()) {
                    show("Help file not found: " + manual);
                }
                else {
                    string cur_line;
                    log("Help file:");
                    while (getline(help, cur_line)) {
                        show(cur_line);
                    }
                    help.close();
                }
            }
        }
        else if (main == "DEF")
        {
            ifstream ccc("default.txt");
            while (!ccc.eof())
            {
                cout << ccc.eof() << "\n";
                ccc >> main;
                cout << main << "\n";
                for (auto& c : main) c = toupper(c);
                if (main == "HELP")
                {
                    string manual;
                    int err = cc->help(id, manual);
                    if (err != 0)
                    {
                        show("Bad help file: " + manual);
                    }
                    else {
                        ifstream help(manual);
                        if (!help.is_open()) {
                            show("Help file not found: " + manual);
                        }
                        else {
                            string cur_Line;
                            log("Help file:");
                            while (getline(help, cur_Line)) {
                                show(cur_Line);
                            }
                            help.close();
                        }
                    }
                }
                else if (main == "GC")
                {
                    if (cc->field_->inf != 2)
                    {
                        double  disp_x, disp_y, c_x, c_y;
                        int dots;
                        ccc >> disp_x >> disp_y >> c_x >> c_y >> dots;
                        if (ccc.fail())
                        {
                            show("Bad values");
                            show("Default cloud created");
                            cc->buff.generate_cloud(0, 0, 1, 1, 100);
                            log("->oblaco sozdano s par-mi: " + to_string(1) + " " + to_string(1) + " " + to_string(0) + " " + to_string(0) + " " + to_string(100));
                            show("HELP-spisok komand");
                            ccc.clear();
                        }
                        else
                        {
                            cc->buff.generate_cloud(c_x, c_y, disp_x, disp_y, dots);
                            show("oblaco sozdano");
                            log("->oblaco sozdano s par-mi" + to_string(disp_x) + " " + to_string(disp_y) + " " + to_string(c_x) + " " + to_string(c_y) + " " + to_string(dots));
                        }
                    }
                    else
                    {
                        show("oshibka:pole v drugom sostoyznii");
                        log("---> oshibka:pole v drugom sostoyznii");
                    }
                }
                else if (main == "GEN")
                {
                    int q;
                    ccc >> q;
                    if (ccc.fail())
                    {
                        show("Bad values");
                        show("Default random buffer created");
                        cc->buff.gen(500);
                        log("->buffer sgenerirovan,tochek: " + to_string(500));
                        ccc.clear();
                    }
                    else
                    {
                        cc->buff.gen(q);
                        show("buffer sgenerirovan");
                        log("buffer sgenerirovan,tochek: " + to_string(q));
                    }
                }
                else if (main == "ROTATE")
                {
                    double  angle;
                    ccc >> angle;
                    if (ccc.fail())
                    {
                        show("Bad values");
                        ccc.clear();
                    }
                    else
                    {
                        cc->buff.rotate(angle);
                        show("buffer povernut");
                        log("buffer povernut na " + to_string(angle));
                    }
                }
                else if (main == "SHIFT")
                {
                    double  x, y;
                    ccc >> x >> y;
                    if (ccc.fail())
                    {
                        show("Bad values");
                        ccc.clear();
                    }
                    else
                    {
                        cc->buff.shift(x, y);
                        show("buffer sdvinut");
                        log("buffer sdvinut na vect " + to_string(x) + to_string(y));
                    }
                }
                else if (main == "LENG")
                {
                    show("dlina pola:%d \n" + to_string(cc->field_->length()));
                }
                else if (main == "UNLOAD")
                {
                    if (cc->field_->inf != 2)
                    {
                        cc->unload();
                        cc->buff.clear();
                        show("buffer vigruzhen");
                        log("->tekushaya dlina pola: " + to_string(cc->field_->length()));
                    }
                    else
                    {
                        show("oshibka:pole v drugom sostoyznii");
                        log("Wrong request");
                    }
                }
                else if (main == "MATRIX")
                {
                    cc->field_->cmatrix();
                    show("matrica sozdana");
                    show("pole perevedeno v sosotoyanie 2(analis)");
                }
                else if (main == "KMC")
                {
                    int k, p;
                    ccc >> k >> p;
                    if (ccc.fail())
                    {
                        show("Bad values");
                        show("KMeans with default values (3,3)");
                        cc->Kmc(3, 3);
                        log("KMeans with values (3,3) printed");
                        ccc.clear();
                    }
                    else
                    {
                        cc->Kmc(k, p);
                        show("proscanirovano(kmcore)");
                        show("pole raspechatano");
                        log("KMeans with values ( " + to_string(k) + "," + to_string(p) + ") printed");
                    }
                }
                else if (main == "FOREL")
                {
                    double r;
                    int dep;
                    ccc >> r >> dep;
                    if (dep < 1)
                    {
                        show("Bad values");
                        show("HELP-spisok komand");
                    }
                    else
                    {
                        if (ccc.fail())
                        {
                            show("Bad values");
                            show("Forel with default value (0.1) and depth (2) printed");
                            cc->FOREL(0.1, 2);
                            log("Forel with value (0.1) and depth (2) printed");
                            ccc.clear();
                        }
                        cc->FOREL(r, dep);
                        show("Forel with value (" + to_string(r) + ") and depth (" + to_string(dep) + ") printed");
                        log("Forel with value (" + to_string(r) + ") and depth (" + to_string(dep) + ") printed");
                    }
                }
                else if (main == "KM")
                {
                    int k;
                    ccc >> k;
                    if (k < 1)
                    {
                        show("Bad values");
                        show("HELP-spisok komand");
                    }
                    else
                    {
                        if (ccc.fail())
                        {
                            show("Bad values");
                            show("Kmeans with default value (5) printed");
                            cc->Km(5);
                            log("Kmeans with value (5) printed");
                            ccc.clear();
                        }
                        cc->Km(k);
                        show("Kmeans with value (" + to_string(k) + ") printed");
                        log("Kmeans with value (" + to_string(k) + ") printed");
                    }
                }
                else if (main == "EM")
                {
                    int k;
                    ccc >> k;
                    if (k < 1)
                    {
                        show("Bad values");
                        show("HELP-spisok komand");
                    }
                    else
                    {
                        if (ccc.fail())
                        {
                            show("Bad values");
                            show("ExpMax with default value (3) printed (gif)");
                            cc->Km(3);
                            log("ExpMax with value (5) printed");
                            ccc.clear();
                        }
                        cc->Km(k);
                        show("ExpMax with value (" + to_string(k) + ") printed (gif)");
                        log("ExpMax with value (" + to_string(k) + ") printed");
                    }
                }
                else if (main == "DBS")
                {
                    if (cc->field_->inf == 2)
                    {
                        double del;
                        int k;
                        ccc >> del >> k;
                        cc->DBSCAN(del, k);
                        show("proscanirovano");
                        log("->proscanirovano(dbs)" + to_string(del) + to_string(k));
                        int err;
                        err = cc->fprintf_db(del, "core.txt", "Line.txt", "dust.txt");
                        if (err == -1)
                        {
                            show("file ne naiden");
                        }
                        else
                        {
                            show("pole raspechatano");
                            log("->pole raspechatano (dbs) " + to_string(del) + " " + to_string(k) + " default");
                            cout << cc->field_->core_.size() << "\n";
                            cout << cc->field_->dust_.size() << "\n";
                        }
                    }
                    else
                    {
                        show("oshibka:pole v drugom sostoyznii");
                        log("oshibka:pole v drugom sostoyznii");
                    }
                }
                else if (main == "IER")
                {
                    cc->IER();
                    show("Hierarchy algorithm printed");
                    log("Hierarchy algorithm printed");
                }
                else if (main == "BINARY")
                {
                    double k;
                    ccc >> k;
                    if (k < 0)
                    {
                        show("Bad values");
                        show("HELP-spisok komand");
                    }
                    else
                    {
                        if (ccc.fail())
                        {
                            show("Bad values");
                            show("Binary matrix with default value (0.1) created");
                            cc->binary(0.1);
                            log("Binary matrix with value (0.1) created");
                            ccc.clear();
                        }
                        else
                        {
                            if (cc->binary(k) < 0)
                            {
                                show("Binary error(no points)");
                                log("Binary error(no points)");
                            }
                            else
                            {
                                show("Binary matrix with (" + to_string(k) + "( created");
                                log("Binary matrix with (" + to_string(k) + "( created");
                            }
                        }
                    }
                }
                else if (main == "WAVE")
                {
                    cc->wave();
                    show("Wave algorithm printed");
                    log("Wave algorithm printed");
                }
                else if (main == "HISTO")
                {
                    {
                        int k;
                        ccc >> k;
                        if (k < 1)
                        {
                            show("Bad values");
                            show("HELP-spisok komand");
                        }
                        else
                        {
                            if (ccc.fail())
                            {
                                show("Bad values");
                                show("Histogramm with default value (30) printed");
                                cc->hist(30);
                                log("Histogramm with value (30) printed");
                                ccc.clear();
                            }
                            cc->hist(k);
                            show("Histogramm with value (" + to_string(k) + ") printed");
                            log("Histogramm with value (" + to_string(k) + ") printed");
                        }
                    }
                }
                else if (main == "TREE")
                {
                    int err;
                    err = cc->tree("tree.txt", 30);
                    show("derevo sozdano");
                    log("->derevo sozdano s hist " + to_string(30) + " default");
                    if (err == -1)
                    {
                        show("file ne naiden");
                    }
                    else
                    {
                        show("derevo raspechatano");
                        log("->derevo  raspechatano : default");
                    }
                }
                else if (main == "DB")
                {
                    //string filename_core,filenme_dust;
                    //ccc >> filename_core>> filenme_dust;
                    //cout << filename_core << ' ' << filenme_dust;
                    //cout << "\n";
                    int err;
                    err = cc->fprintf_db(0.05, "core.txt", "Line.txt", "dust.txt");
                    if (err == -1)
                    {
                        show("file ne naiden");
                    }
                    else
                    {
                        show("pole raspechatano");
                    }
                }
                else {
                    show("unknown command in file");
                    show("HELP-spisok komand");
                }
            }
        }
        else if (main == "C")
        {
            string filename; cin >> filename;
            ifstream ccc(filename);
            while (!ccc.eof())
            {
                ccc >> main;
                for (auto& c : main) c = toupper(c);
                if (main == "HELP")
                {
                    string manual;
                    int err = cc->help(id, manual);
                    if (err != 0)
                    {
                        show("Bad help file: " + manual);
                    }
                    else {
                        ifstream help(manual);
                        if (!help.is_open()) {
                            show("Help file not found: " + manual);
                        }
                        else {
                            string cur_Line;
                            log("Help file:");
                            while (getline(help, cur_Line)) {
                                show(cur_Line);
                            }
                            help.close();
                        }
                    }
                }
                else if (main == "GC")
                {
                    if (cc->field_->inf != 2)
                    {
                        double  disp_x, disp_y, c_x, c_y;
                        int dots;
                        ccc >> disp_x >> disp_y >> c_x >> c_y >> dots;
                        if (ccc.fail())
                        {
                            show("Bad values");
                            show("Default cloud created");
                            cc->buff.generate_cloud(0, 0, 1, 1, 100);
                            log("->oblaco sozdano s par-mi: " + to_string(1) + " " + to_string(1) + " " + to_string(0) + " " + to_string(0) + " " + to_string(100));
                            show("HELP-spisok komand");
                            ccc.clear();
                        }
                        else
                        {
                            cc->buff.generate_cloud(c_x, c_y, disp_x, disp_y, dots);
                            show("oblaco sozdano");
                            log("->oblaco sozdano s par-mi" + to_string(disp_x) + " " + to_string(disp_y) + " " + to_string(c_x) + " " + to_string(c_y) + " " + to_string(dots));
                        }
                    }
                    else
                    {
                        show("oshibka:pole v drugom sostoyznii");
                        log("---> oshibka:pole v drugom sostoyznii");
                    }
                }
                else if (main == "GEN")
                {
                    int q;
                    ccc >> q;
                    if (ccc.fail())
                    {
                        show("Bad values");
                        show("Default random buffer created");
                        cc->buff.gen(500);
                        log("->buffer sgenerirovan,tochek: " + to_string(500));
                        ccc.clear();
                    }
                    else
                    {
                        cc->buff.gen(q);
                        show("buffer sgenerirovan");
                        log("buffer sgenerirovan,tochek: " + to_string(q));
                    }
                }
                else if (main == "ROTATE")
                {
                    double  angle;
                    ccc >> angle;
                    if (ccc.fail())
                    {
                        show("Bad values");
                        ccc.clear();
                    }
                    else
                    {
                        cc->buff.rotate(angle);
                        show("buffer povernut");
                        log("buffer povernut na " + to_string(angle));
                    }
                }
                else if (main == "SHIFT")
                {
                    double  x, y;
                    ccc >> x >> y;
                    if (ccc.fail())
                    {
                        show("Bad values");
                        ccc.clear();
                    }
                    else
                    {
                        cc->buff.shift(x, y);
                        show("buffer sdvinut");
                        log("buffer sdvinut na vect " + to_string(x) + to_string(y));
                    }
                }
                else if (main == "LENG")
                {
                    show("dlina pola:%d \n" + to_string(cc->field_->length()));
                }
                else if (main == "UNLOAD")
                {
                    if (cc->field_->inf != 2)
                    {
                        cc->unload();
                        cc->buff.clear();
                        show("buffer vigruzhen");
                        log("->tekushaya dlina pola: " + to_string(cc->field_->length()));
                    }
                    else
                    {
                        show("oshibka:pole v drugom sostoyznii");
                        log("Wrong request");
                    }
                }
                else if (main == "MATRIX")
                {
                    cc->field_->cmatrix();
                    show("matrica sozdana");
                    show("pole perevedeno v sosotoyanie 2(analis)");
                }
                else if (main == "KMC")
                {
                    int k, p;
                    ccc >> k >> p;
                    if (ccc.fail())
                    {
                        show("Bad values");
                        show("KMeans with default values (3,3)");
                        cc->Kmc(3, 3);
                        log("KMeans with values (3,3) printed");
                        ccc.clear();
                    }
                    else
                    {
                        cc->Kmc(k, p);
                        show("proscanirovano(kmcore)");
                        show("pole raspechatano");
                        log("KMeans with values ( " + to_string(k) + "," + to_string(p) + ") printed");
                    }
                }
                else if (main == "FOREL")
                {
                    double r;
                    int dep;
                    ccc >> r >> dep;
                    if (dep < 1)
                    {
                        show("Bad values");
                        show("HELP-spisok komand");
                    }
                    else
                    {
                        if (ccc.fail())
                        {
                            show("Bad values");
                            show("Forel with default value (0.1) and depth (2) printed");
                            cc->FOREL(0.1, 2);
                            log("Forel with value (0.1) and depth (2) printed");
                            ccc.clear();
                        }
                        cc->FOREL(r, dep);
                        show("Forel with value (" + to_string(r) + ") and depth (" + to_string(dep) + ") printed");
                        log("Forel with value (" + to_string(r) + ") and depth (" + to_string(dep) + ") printed");
                    }
                }
                else if (main == "KM")
                {
                    int k;
                    ccc >> k;
                    if (k < 1)
                    {
                        show("Bad values");
                        show("HELP-spisok komand");
                    }
                    else
                    {
                        if (ccc.fail())
                        {
                            show("Bad values");
                            show("Kmeans with default value (5) printed");
                            cc->Km(5);
                            log("Kmeans with value (5) printed");
                            ccc.clear();
                        }
                        cc->Km(k);
                        show("Kmeans with value (" + to_string(k) + ") printed");
                        log("Kmeans with value (" + to_string(k) + ") printed");
                    }
                }
                else if (main == "EM")
                {
                    int k;
                    ccc >> k;
                    if (k < 1)
                    {
                        show("Bad values");
                        show("HELP-spisok komand");
                    }
                    else
                    {
                        if (ccc.fail())
                        {
                            show("Bad values");
                            show("ExpMax with default value (3) printed (gif)");
                            cc->Km(3);
                            log("ExpMax with value (5) printed");
                            ccc.clear();
                        }
                        cc->Km(k);
                        show("ExpMax with value (" + to_string(k) + ") printed (gif)");
                        log("ExpMax with value (" + to_string(k) + ") printed");
                    }
                }
                else if (main == "DBS")
                {
                    if (cc->field_->inf == 2)
                    {
                        double del;
                        int k;
                        ccc >> del >> k;
                        cc->DBSCAN(del, k);
                        show("proscanirovano");
                        log("->proscanirovano(dbs)" + to_string(del) + to_string(k));
                        int err;
                        err = cc->fprintf_db(del, "core.txt", "Line.txt", "dust.txt");
                        if (err == -1)
                        {
                            show("file ne naiden");
                        }
                        else
                        {
                            show("pole raspechatano");
                            log("->pole raspechatano (dbs) " + to_string(del) + " " + to_string(k) + " default");
                            cout << cc->field_->core_.size() << "\n";
                            cout << cc->field_->dust_.size() << "\n";
                        }
                    }
                    else
                    {
                        show("oshibka:pole v drugom sostoyznii");
                        log("oshibka:pole v drugom sostoyznii");
                    }
                }
                else if (main == "IER")
                {
                    cc->IER();
                    show("Hierarchy algorithm printed");
                    log("Hierarchy algorithm printed");
                }
                else if (main == "BINARY")
                {
                    double k;
                    ccc >> k;
                    if (k < 0)
                    {
                        show("Bad values");
                        show("HELP-spisok komand");
                    }
                    else
                    {
                        if (ccc.fail())
                        {
                            show("Bad values");
                            show("Binary matrix with default value (0.1) created");
                            cc->binary(0.1);
                            log("Binary matrix with value (0.1) created");
                            ccc.clear();
                        }
                        else
                        {
                            if (cc->binary(k) < 0)
                            {
                                show("Binary error(no points)");
                                log("Binary error(no points)");
                            }
                            else
                            {
                                show("Binary matrix with (" + to_string(k) + "( created");
                                log("Binary matrix with (" + to_string(k) + "( created");
                            }
                        }
                    }
                }
                else if (main == "WAVE")
                {
                    cc->wave();
                    show("Wave algorithm printed");
                    log("Wave algorithm printed");
                }
                else if (main == "HISTO")
                {
                    {
                        int k;
                        ccc >> k;
                        if (k < 1)
                        {
                            show("Bad values");
                            show("HELP-spisok komand");
                        }
                        else
                        {
                            if (ccc.fail())
                            {
                                show("Bad values");
                                show("Histogramm with default value (30) printed");
                                cc->hist(30);
                                log("Histogramm with value (30) printed");
                                ccc.clear();
                            }
                            cc->hist(k);
                            show("Histogramm with value (" + to_string(k) + ") printed");
                            log("Histogramm with value (" + to_string(k) + ") printed");
                        }
                    }
                }
                else if (main == "TREE")
                {
                    int err;
                    err = cc->tree("tree.txt", 30);
                    show("derevo sozdano");
                    log("->derevo sozdano s hist " + to_string(30) + " default");
                    if (err == -1)
                    {
                        show("file ne naiden");
                    }
                    else
                    {
                        show("derevo raspechatano");
                        log("->derevo  raspechatano : default");
                    }
                }
                else if (main == "DB")
                {
                    //string filename_core,filenme_dust;
                    //ccc >> filename_core>> filenme_dust;
                    //cout << filename_core << ' ' << filenme_dust;
                    //cout << "\n";
                    int err;
                    err = cc->fprintf_db(0.05, "core.txt", "Line.txt", "dust.txt");
                    if (err == -1)
                    {
                        show("file ne naiden");
                    }
                    else
                    {
                        show("pole raspechatano");
                    }
                }
                else {
                    show("unknown command in file");
                    show("HELP-spisok komand");
                }
            }
        }
        else if (main == "L")
        {
            vector <Point> p;
            Point p11(0, 0);
            Point p12(2, 2);
            Point p21(0, 2);
            Point p22(2, 0);
            Point a(0, 0); Point b(1, 1);
            p.emplace_back(p11); p.emplace_back(p21); p.emplace_back(p12); p.emplace_back(p22);
            Poly o(p);
            Line l (a, b);
            cout << l.k << "\n";
            cout << l.b << "\n";
            Hplane s(l, p12);
            cout << LF(p12, s) << "\n";
            Poly f=sec(o, s);
            for (int i = 0; i < f.q; ++i)
                cout << f.point_[i].x_ << " " << f.point_[i].y_ << "\n";
        }
        else if (main == "GC")
        {
            if (cc->field_->inf != 2)
            {
                double  disp_x, disp_y, c_x, c_y;
                int dots;
                cin >> disp_x >> disp_y >> c_x >> c_y >> dots;
                if (cin.fail())
                {
                    show("Bad values");
                    show("Default cloud created");
                    cc->buff.generate_cloud(0, 0, 1, 1, 100);
                    log("->oblaco sozdano s par-mi: " + to_string(1) + " " + to_string(1) + " " + to_string(0) + " " + to_string(0) + " " + to_string(100));
                    show("HELP-spisok komand");
                    cin.clear();
                }
                else
                {
                    cc->buff.generate_cloud(c_x, c_y, disp_x, disp_y, dots);
                    show("oblaco sozdano");
                    log("->oblaco sozdano s par-mi" + to_string(disp_x) + " " + to_string(disp_y) + " " + to_string(c_x) + " " + to_string(c_y) + " " + to_string(dots));
                }
            }
            else
            {
                show("oshibka:pole v drugom sostoyznii");
                log("---> oshibka:pole v drugom sostoyznii");
            }
        }
        else if (main == "GEN")
        {
            int q;
            cin >> q;
            if (cin.fail())
            {
                show("Bad values");
                show("Default random buffer created");
                cc->buff.gen(500);
                log("->buffer sgenerirovan,tochek: " + to_string(500));
                cin.clear();
            }
            else
            {
                cc->buff.gen(q);
                show("buffer sgenerirovan");
                log("buffer sgenerirovan,tochek: " + to_string(q));
            }
        }
        else if (main == "ROTATE")
        {
            double  angle;
            cin >> angle;
            if (cin.fail())
            {
                show("Bad values");
                cin.clear();
            }
            else
            {
                cc->buff.rotate(angle);
                show("buffer povernut");
                log("buffer povernut na " + to_string(angle));
            }
        }
        else if (main == "SHIFT")
        {
            double  x, y;
            cin >> x >> y;
            if (cin.fail())
            {
                show("Bad values");
                cin.clear();
            }
            else
            {
                cc->buff.shift(x, y);
                show("buffer sdvinut");
                log("buffer sdvinut na vect " + to_string(x) + to_string(y));
            }
        }
        else if (main == "LENG")
        {
            show("Field Length" + to_string(cc->field_->length()));
        }
        else if (main == "UNLOAD")
        {
            if (cc->field_->inf != 2)
            {
                cc->unload();
                cc->buff.clear();
                show("buffer vigruzhen");
                log("->tekushaya dlina pola: " + to_string(cc->field_->length()));
            }
            else
            {
                show("oshibka:pole v drugom sostoyznii");
                log("Wrong request");
            }
        }
        else if (main == "MATRIX")
        {
            cc->field_->cmatrix();
            show("matrica sozdana");
            show("pole perevedeno v sosotoyanie 2(analis)");
        }
        else if (main == "KMC")
        {
            int k, p;
            cin >> k >> p;
            if (cin.fail())
            {
                show("Bad values");
                show("KMeans with default values (3,3)");
                cc->Kmc(3, 3);
                log("KMeans with values (3,3) printed");
                cin.clear();
            }
            else
            {
                cc->Kmc(k, p);
                show("proscanirovano(kmcore)");
                show("pole raspechatano");
                log("KMeans with values ( " + to_string(k) + "," + to_string(p) + ") printed");
            }
        }
        else if (main == "FOREL")
        {
            double r;
            int dep;
            cin >> r >> dep;
            if (dep < 1)
            {
                show("Bad values");
                show("HELP-spisok komand");
            }
            else
            {
                if (cin.fail())
                {
                    show("Bad values");
                    show("Forel with default value (0.1) and depth (2) printed");
                    cc->FOREL(0.1, 2);
                    log("Forel with value (0.1) and depth (2) printed");
                    cin.clear();
                }
                cc->FOREL(r, dep);
                show("Forel with value (" + to_string(r) + ") and depth (" + to_string(dep) + ") printed");
                log("Forel with value (" + to_string(r) + ") and depth (" + to_string(dep) + ") printed");
            }
        }
        else if (main == "KM")
        {
            int k;
            cin >> k;
            if (k < 1)
            {
                show("Bad values");
                show("HELP-spisok komand");
            }
            else
            {
                if (cin.fail())
                {
                    show("Bad values");
                    show("Kmeans with default value (5) printed");
                    cc->Km(5);
                    log("Kmeans with value (5) printed");
                    cin.clear();
                }
                cc->Km(k);
                show("Kmeans with value (" + to_string(k) + ") printed");
                log("Kmeans with value (" + to_string(k) + ") printed");
            }
        }
        else if (main == "EM")
        {
            int k;
            cin >> k;
            if (k < 1)
            {
                show("Bad values");
                show("HELP-spisok komand");
            }
            else
            {
                if (cin.fail())
                {
                    show("Bad values");
                    show("ExpMax with default value (3) printed (gif)");
                    cc->Km(3);
                    log("ExpMax with value (5) printed");
                    cin.clear();
                }
                cc->Km(k);
                show("ExpMax with value (" + to_string(k) + ") printed (gif)");
                log("ExpMax with value (" + to_string(k) + ") printed");
            }
        }
        else if (main == "DBS")
        {
            if (cc->field_->inf == 2)
            {
                double del;
                int k;
                cin >> del >> k;
                cc->DBSCAN(del, k);
                show("proscanirovano");
                log("->proscanirovano(dbs)" + to_string(del) + to_string(k));
                int err;
                err = cc->fprintf_db(del, "core.txt", "Line.txt", "dust.txt");
                if (err == -1)
                {
                    show("file ne naiden");
                }
                else
                {
                    show("pole raspechatano");
                    log("->pole raspechatano (dbs) " + to_string(del) + " " + to_string(k) + " default");
                }
            }
            else
            {
                show("oshibka:pole v drugom sostoyznii");
                log("oshibka:pole v drugom sostoyznii");
            }
        }
        else if (main == "IER")
        {
            cc->IER();
            show("Hierarchy algorithm printed");
            log("Hierarchy algorithm printed");
        }
        else if (main == "VORONOI")
        {
            cc->inter();
            show("Voronoi diagram printed");
            log("Voronoi diagram printed");
        }
        else if (main == "BINARY")
        {
            double k;
            cin >> k;
            if (k < 0)
            {
                show("Bad values");
                show("HELP-spisok komand");
            }
            else
            {
                if (cin.fail())
                {
                    show("Bad values");
                    show("Binary matrix with default value (0.1) created");
                    cc->binary(0.1);
                    log("Binary matrix with value (0.1) created");
                    cin.clear();
                }
                else
                {
                    if (cc->binary(k) < 0)
                    {
                        show("Binary error(no points)");
                        log("Binary error(no points)");
                    }
                    else
                    {
                        show("Binary matrix with (" + to_string(k) + "( created");
                        log("Binary matrix with (" + to_string(k) + "( created");
                    }
                }
            }
        }
        else if (main == "WAVE")
        {
            cc->wave();
            show("Wave algorithm printed");
            log("Wave algorithm printed");
        }
        else if (main == "HISTO")
        {
            {
                int k;
                cin >> k;
                if (k < 1)
                {
                    show("Bad values");
                    show("HELP-spisok komand");
                }
                else
                {
                    if (cin.fail())
                    {
                        show("Bad values");
                        show("Histogramm with default value (30) printed");
                        cc->hist(30);
                        log("Histogramm with value (30) printed");
                        cin.clear();
                    }
                    cc->hist(k);
                    show("Histogramm with value (" + to_string(k) + ") printed");
                    log("Histogramm with value (" + to_string(k) + ") printed");
                }
            }
        }
        else if (main == "TREE")
        {
            int err;
            err = cc->tree("tree.txt", 30);
            show("derevo sozdano");
            log("->derevo sozdano s hist " + to_string(30) + " default");
            if (err == -1)
            {
                show("file ne naiden");
            }
            else
            {
                show("derevo raspechatano");
                log("->derevo  raspechatano : default");
            }
        }
        else if (main == "DB")
        {
            //string filename_core,filenme_dust;
            //cin >> filename_core>> filenme_dust;
            //cout << filename_core << ' ' << filenme_dust;
            //cout << "\n";
            int err;
            err = cc->fprintf_db(0.05, "core.txt", "Line.txt", "dust.txt");
            if (err == -1)
            {
                show("file ne naiden");
            }
            else
            {
                show("pole raspechatano");
            }
        }
        else {
            show("neizvestnaya comanda");
            show("HELP-spisok komand");
        }
        cin >> main;
        for (auto& c : main) c = toupper(c);
    }
    return 0;
}

void Interface::log(const string& s)
{
    if (write_log)
    {
        struct tm* a;
        const time_t timer = time(NULL);
        a = localtime(&timer);
        logger << a->tm_mday << "." << (a->tm_mon + 1) << " " << (a->tm_hour) << ":" << (a->tm_min) << ":" << (a->tm_sec) << " -- " << s << endl;
    }
}

void Interface::show(const string& s)
{
    cout << s << endl;
    log("<< " + s);
}