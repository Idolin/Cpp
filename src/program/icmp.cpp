#include <sys/stat.h>
#include <cstdio>
#include <algorithm>
#include <atomic>
#include <dirent.h>
#include <cmath>
#include <exception>
#include <pthread.h>
#include <unistd.h>
#include <condition_variable>
#include <csignal>
#include <chrono>
#include <sys/wait.h>
#include <cstdarg>
#include "../ImageMagick-7/Magick++.h"
#include "../string/cstr.h"
#include "../container/vector.hpp"
#include "../other/common.hpp"
#include "../template/arraymethods.hpp"
#include "../algo/icmp_normalize.h"
#include "../other/pam_writer.h"

static void update_linef(const char *format, ...)
{
    putchar('\r');
    va_list va;
    va_start(va, format);
    vprintf(format, va);
    va_end(va);
    fflush(stdout);
}

//#define SECOND_METHOD
#ifdef SECOND_METHOD
    static const bool method_efficiency = true;
#else
    static const bool method_efficiency = false;
#endif
using namespace Magick;
using namespace std::chrono_literals;

bool is_picture(const cstr &name)
{
    static const cstr extensions[] = {".jpeg", ".jpg", ".png", ".bmp", ".xmp", ".svg", ".gif", ".xcf"};
    for(const cstr& ext : extensions)
        if(name.endswith(ext))
            return true;
    return false;
}

struct picture
{
    cstr path, name, inode;
};

unsigned char file_type(const cstr &name, ino_t &inode)  //0 - not exists, 1 - regular file, 2 - directory, 3 - something else
{
    struct stat buffer; //NOLINT (will be initialized in stat)
    if(stat(name.c_str(), &buffer))
        return 0;
    else
    {
        inode = buffer.st_ino;
        if(buffer.st_mode & S_IFREG)
            return 1;
        elif(buffer.st_mode & S_IFDIR)
            return 2;
        else
            return 3;
    }
}

inline unsigned char file_type(const cstr &name)
{
    ino_t ignored;
    return file_type(name, ignored);
}

bool is_program(const cstr &name)
{
    if(name.find(" ") != cstr::not_found)
        return false;
    cstr no_output_compose = cstr("hash ") + name + " &>/dev/null";
    return (system(no_output_compose.c_str()) == 0);
}

vect<std::tuple<cstr, cstr, unsigned char>> to_show;
std::mutex show_lock;
std::condition_variable show_cond;
std::atomic<std::size_t> elements_amount {0};
bool show_diff = false;

cstr tmp_dir = "/tmp/diff/", ext = ".pgm", fs_del = "/", dot = ".", application = "";

void* pictures_show(void*)
{
    Geometry g(512, 512, 0, 0);
    g.aspect(true);
    std::size_t i = 0;
    ino_t ignore;
    bool something_was_shown = false;
    while(true)
    {
        {
            std::unique_lock<std::mutex> s_lock(show_lock);
            while(i == to_show.size())
                show_cond.wait(s_lock);
        }
        std::tuple<cstr, cstr, double> e = to_show[i++];
        if(std::get<0>(e).length() == 0)
            break;
        if(file_type(std::get<0>(e), ignore) == 0 || file_type(std::get<1>(e), ignore) == 0)
            continue;
        Image i1, i2, diff;
        if(show_diff && std::get<2>(e) <= 100)
        {
            i1.read(std::get<0>(e));
            i2.read(std::get<1>(e));
            i1.scale(g);
            i2.scale(g);
            i1.write(tmp_dir + "1.bmp");
            i2.write(tmp_dir + "2.bmp");
            double fuzz = (100 - min(static_cast<double>(std::get<2>(e)), static_cast<double>(100))) / 1000;
            i1.colorFuzz(fuzz * QuantumRange);
            diff = i1.compare(i2, FuzzErrorMetric, &fuzz);
            diff.write(tmp_dir + "1d2.bmp");
        }
        pid_t notification = fork();
        if(notification == 0)
        {
            cstr diff_msg = std::get<2>(e) == 101 ? cstr("=") : cstr("~");
            auto diff_value = min(std::get<2>(e), 100);
            cstr message = std::get<0>(e);
            message += " ";
            message += diff_msg + "=(";
            message += cstr(diff_value);
            message += "%)=";
            message += diff_msg + " ";
            message += std::get<1>(e);
            diff_msg = cstr(diff_value) + "% (" + cstr(i) + "/" + cstr(elements_amount.load()) + ")";
            close(1);
            close(2);
            execlp("notify-send", "notification", diff_msg.c_str(), message.c_str(), NULL);
        }
        pid_t p1, p2, p3 = 0;
        p1 = fork();
        if(p1 == 0)
        {
            close(1);
            close(2);
            execlp("gwenview", "show", std::get<0>(e).c_str(), NULL);
        }
        usleep(100000); // sleep for 100ms
        p2 = fork();
        if(p2 == 0)
        {
            close(1);
            close(2);
            execlp("gwenview", "show", std::get<1>(e).c_str(), NULL);
        }
        if(show_diff && std::get<2>(e) <= 100)
        {
            usleep(100000); // sleep for 100ms
            p3 = fork();
            if(p3 == 0)
            {
                close(1);
                close(2);
                execlp("gwenview", "diff", cstr(tmp_dir + cstr("1d2.bmp")).c_str(), NULL);
            }
        }
        while(true)
        {
            usleep(1000);
            pid_t pt = waitpid(-1, nullptr, WNOHANG);
            if(pt > 0 && pt != p3 && pt != notification)
                break;
            if(file_type(std::get<0>(e), ignore) == 0 || file_type(std::get<1>(e), ignore) == 0)
                break;
        }
        kill(p1, SIGKILL);
        kill(p2, SIGKILL);
        if(show_diff && std::get<2>(e) <= 100)
        {
            remove(tmp_dir + "1.bmp");
            remove(tmp_dir + "2.bmp");
            remove(tmp_dir + "1d2.bmp");
            kill(p3, SIGKILL);
        }
        while(wait(nullptr) > 0); // wait for all childs to finish
        something_was_shown = true;
    }
    if(something_was_shown)
    {
        pid_t notification = fork();
        if(notification == 0)
        {
            close(1);
            close(2);
            execlp("notify-send", "notification", "No more alike pictures", "", NULL);
        }
        waitpid(notification, nullptr, 0);
    }
    return nullptr;
}

double get_coeff(cstr file_name)
{
    FILE *img = fopen(file_name.c_str(), "rb");
    ASSERT(img != nullptr, "Can't read from file: %s", file_name.c_str());
    char buffer[100]{};
    double res = -1;
    size_t r = fread(&buffer, 100, 1, img);
    if((r == 1) && c_str_equals(buffer, "P5\n#Coefficient:", 16))
        for(int i = 16;i < 90;i++)
            if(buffer[i] == '\n')
            {
                if(!sscanf(buffer + 16, "%lf", &res))
                    res = -1;
                break;
            }
    fclose(img);
    return res;
}

struct DirOpenException: std::exception
{
private:
    const char *message;

public:
    DirOpenException(cstr dir_path)
    {
       message = cstr("Can't open directory: ") + dir_path;
    }

    const char* what() const noexcept override
    {
        return message;
    }
};

bool scan_dir(const char *dir_path, vect<picture> &files, bool recursive = false)
{
    bool contains_pictures = false;
    ino_t inode;
    DIR *dir;
    struct dirent *ent;
    if((dir = opendir(dir_path)) != nullptr)
    {
        while((ent = readdir(dir)) != nullptr)
        {
            cstr d_name = ent->d_name;
            if((d_name == ".") || (d_name == ".."))
                continue;
            cstr full_path = cstr(dir_path) + fs_del + d_name;
            unsigned char type = file_type(full_path, inode);
            if(type == 1)
            {
                if(is_picture(d_name))
                    files.push(picture{full_path, d_name, cstr(inode)}), contains_pictures = true;
            }
            else if(recursive && (type == 2))
                contains_pictures |= scan_dir(full_path.c_str(), files, recursive);
        }
        closedir(dir);
    }
    else
        throw DirOpenException(dir_path);
    return contains_pictures;
}



int main(int argc, char **argv)
{
    InitializeMagick(nullptr);
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    int i, range_i = 0;
    // unsigned char threads_amount = 4;
    double sensibility = 25.0;
    vect<std::pair<cstr, bool>> dirs_and_files;
    bool unique = false, recursive = false, file_arguments = false, clear_cache = false, all_ok = true, show_in_background = false;
    bool symmetry_h = false, symmetry_v = false, symmetry;
    bool show_on = true, readable_format = false;
    for(const char* application_try : {"eog", "gwenview", "feh"})
        if(is_program(application_try))
        {
            application = application_try;
            break;
        }
    for(i = 1; i < argc; i++)
        if(file_arguments || argv[i][0] != '-')
        {
            file_arguments = true;
            cstr filepath = cstr(argv[i]);
            unsigned cut_to = filepath.length();
            while(cut_to > 0)
                if(filepath[--cut_to] != '/')
                {
                    cut_to++;
                    break;
                }
            filepath = filepath.substr(0, cut_to);
            dirs_and_files.push(std::make_pair(filepath, false));
        }
        else
        {
            if(c_str_equals(argv[i], "-u") || c_str_equals(argv[i], "--unique"))
                unique = true;
            elif(c_str_equals(argv[i], "-r") || c_str_equals(argv[i], "--recursive"))
                recursive = true;
            elif(c_str_equals(argv[i], "-N") || c_str_equals(argv[i], "--not-show"))
                show_on = false;
            elif(c_str_equals(argv[i], "-1") || c_str_equals(argv[i], "--readable"))
                readable_format = true;
            elif(c_str_equals(argv[i], "-d") || c_str_equals(argv[i], "--diff"))
                show_diff = true, show_on = true;
            elif(c_str_equals(argv[i], "-c") || c_str_equals(argv[i], "--clear-cache"))
                clear_cache = true;
            elif(c_str_equals(argv[i], "-t") || c_str_equals(argv[i], "--tmp"))
            {
                if(++i == argc)
                    fprintf(stderr, "Expected path to directory after %s option, but no argument followed\n", argv[i - 1]), all_ok = false;
                else
                    tmp_dir = cstr(argv[i]) + "/";
                if(all_ok && file_type(tmp_dir) != 2)
                    fprintf(stderr, "No such directory: %s\n", tmp_dir.c_str()), all_ok = false;
                if(all_ok && (access(tmp_dir.c_str(), R_OK  | W_OK)))
                    fprintf(stderr, "Don't have access to write(read) to directory: %s\n", tmp_dir.c_str()), all_ok = false;
            }
            elif(c_str_equals(argv[i], "-n") || c_str_equals(argv[i], "--no-wait"))
                show_in_background = true;
            elif(c_str_equals(argv[i], "-a") || c_str_equals(argv[i], "--app"))
            {
                if(++i == argc)
                    fprintf(stderr, "Expected application name after %s option, but no argument followed\n", argv[i - 1]), all_ok = false;
                else
                    application = argv[i];
            }
            elif(c_str_equals(argv[i], "-s") || c_str_equals(argv[i], "--sensibility"))
            {
                if(++i == argc)
                    fprintf(stderr, "Expected sensibility number after %s option, but no argument followed\n", argv[i - 1]), all_ok = false;
                elif(sscanf(argv[i], "%lf", &sensibility) != 1)
                    fprintf(stderr, "Can't parse '%s' as sensibility number\n", argv[i]), all_ok = false;
                if(all_ok && (sensibility < 0 || sensibility > 100))
                    fprintf(stderr, "Sensibility number expected to be in range from 0 to 100, but not %lf\n", sensibility), all_ok = false;
            }
            elif(c_str_equals(argv[i], "-A") || c_str_equals(argv[i], "--symmetry") || c_str_equals(argv[i], "--symmetry-all"))
                symmetry_h = symmetry_v = true;
            elif(c_str_equals(argv[i], "-H") || c_str_equals(argv[i], "--symmetry-horizontal"))
                symmetry_h = true;
            elif(c_str_equals(argv[i], "-V") || c_str_equals(argv[i], "--symmetry-vertical"))
                symmetry_v = true;
            elif(c_str_equals(argv[i], "--"))
                file_arguments = true;
            elif(c_str_equals(argv[i], "-h") || c_str_equals(argv[i], "--help"))
            {
                printf("icmp - program for comparing images\n" // [TODO:FDFT,NTT?]
                       "Usage:\n"
                       "\ticmp [options] [--] (file1|dir1) [file2|dir2] ... [fileN|dirN]\n"
                       "Options:\\n\\\n"
                       "\t-h or --help: show this help and exit\n"
                       "\t-u or --unique: assume what every given directory contain only unique images\n"
                       "\t-r or --recursive: recursively scan directories for pictures to compare\n"
                       "\t-s or --sensibility <float>: count two pictures different if they similarity more than given number percents[25 by default]\n"
                       "\t-a or --app <program_name>: choose a program for showing pictures\n"
                       "\t-d or --diff: show difference between pictures\n"
                       "\t-N or --not-show: don't show pictures\n"
                       "\t-1 or --readable: print info on console in readable format ({filename, diff percent, filename} divided by newline)\n"
                       "\t-n or --no-wait: don't wait for showing process, exit immediately after comparison\n"
                       "\t-t or --tmp <dir>: choose tmp directory(/tmp/diff for default)\n"
                       "\t-c or --clear-cache: clear converted pictures cache at exit\n"
                       "\t-[AHV] or --symmetry((-all)?|-horizontal|-vertical): compare mirrored images as well\n"
                       "\t--threads <number>: use specified number of threads[TODO]\n"
                       "\t--: interpret following arguments as files and directories\n");
                return 0;
            }
            else
            {
                fprintf(stderr, "Unknown option: %s\n", argv[i]);
                return 1;
            }
        }
    if(show_on && !is_program(application))
        fprintf(stderr, application.length() > 0 ? "No such program: %s\n" : "Can't find a program to show pictures%s\n", application.c_str()), all_ok = false;
    if(!all_ok)
        return 1;
    symmetry = symmetry_h | symmetry_v;
    if(clear_cache || method_efficiency)
    {
        if(tmp_dir.startswith("/tmp/"))
            system(cstr("rm -rf ") + tmp_dir);
        else
            fprintf(stderr, "Can remove cache only placed in /tmp directory\n");
    }
    if(method_efficiency)
        unique = false, show_on = false, dirs_and_files.clear(), dirs_and_files.push(std::make_pair("/home/cynder/Изображения/pic8/ICMPTests/TestDir", false));
    vect<picture> files;
    unsigned *unique_ranges = unique ? new unsigned[dirs_and_files.size()] : nullptr;

    for(std::pair<cstr, bool>& f : dirs_and_files)
    {
        ino_t inode;
        unsigned char type = file_type(f.first, inode);
        if(!(type))
            fprintf(stderr, "File or directory not found: %s\n", f.first.c_str()), all_ok = false;
        elif(type == 2)
            f.second = true;
        elif(type != 1)
            fprintf(stderr, "Not a file or directory: %s\n", f.first.c_str()), all_ok = false;
        elif(!is_picture(f.first))
            fprintf(stderr, "File doesn't seems to be a picture: %s\n", f.first.c_str()), all_ok = false;
        elif(all_ok)
        {
            files.push(picture{f.first, f.first.substr(f.first.rfind(fs_del) + 1), cstr(inode)});
            if(unique)
                unique_ranges[range_i++] = static_cast<unsigned>(files.size());
        }
    }

    if(!all_ok)
    {
        delete[] unique_ranges;
        return 1;
    }

    try
    {
        for(const std::pair<cstr, bool>& f : dirs_and_files)
        {
            if(f.second)
            {
                if(!scan_dir(f.first.c_str(), files, recursive))
                {
                    fprintf(stderr, "No pictures found in the directory: %s\n", f.first.c_str());
                    all_ok = false;
                    break;
                }
                if(unique)
                    unique_ranges[range_i++] = static_cast<unsigned>(files.size());
            }
        }
    }
    catch(const DirOpenException &exception)
    {
        fputs(exception.what(), stderr);
        all_ok = false;
    }

    if(!all_ok)
    {
        delete[] unique_ranges;
        return 1;
    }
    unsigned long long comparisons = 0;
    if(unique)
        for(i = 1;i < range_i;i++)
            comparisons += (unique_ranges[i] - unique_ranges[i - 1]) * unique_ranges[i - 1];
    else
        comparisons = static_cast<unsigned long long>(files.size()) * (files.size() - 1) >> 1u;
    if(comparisons == 0)
    {
        delete[] unique_ranges;
        fprintf(stderr, "Nothing to compare\n");
        return 1;
    }
    if(!readable_format)
        printf("Comparisons: %llu\n", comparisons);
    if(file_type(tmp_dir) == 0)
        mkdir(tmp_dir.c_str(), 0777);
    unsigned wh = 64;
    Geometry g1(wh, wh, 0, 0);
    g1.aspect(true);
    unsigned char *pixels = new unsigned char[wh * wh],
            *pixels_diff = new unsigned char[wh * wh];
    double *coeff = new double[files.size()];
#ifdef SECOND_METHOD
    double *coeff2 = new double[files.size()];
#endif
    unsigned converted = 0;
    for(unsigned file_i = 0; file_i < files.size(); file_i++)
    {
        if(!readable_format)
            update_linef("Converting: %u/%lu", converted++, files.size());
        if(file_type(tmp_dir + files[file_i].name + dot + files[file_i].inode + ext) &&
           ((coeff[file_i] = get_coeff(tmp_dir + files[file_i].name + dot + files[file_i].inode + ext)) >= 0))
            continue;
        try
        {
            coeff[file_i] = image_convert_diff(files[file_i].path.c_str(), pixels_diff);
            PAMImageWriter writer(PAMImageWriter::keep_exact(tmp_dir + files[file_i].name + dot + files[file_i].inode + ext), wh, wh);
            writer.set_comment(cstr("Coefficient:") + std::to_string(coeff[file_i]));
            writer.write(pixels_diff);
        }
        catch(std::exception &exception)
        {
            if(!readable_format)
                update_linef("");
            fprintf(stderr, "Exception occurred while processing following image: %s\nImage will be ignored\n", files[file_i].path.c_str());
            fprintf(stderr, "what(): %s\n", exception.what());
            files[file_i].path = "";
            continue;
        }
////        for(unsigned si = 0;si < wh * wh;si++)
////        {
////            int pd = pixels_diff[si];
////            int npd = pd;
////            pixels_diff[si] = static_cast<unsigned char>(npd <= 0 ? 0 : (npd >= 256 ? 255 : npd));
////        }
////        sm = _sum<unsigned>(pixels_diff, wh * wh);
//#ifdef SECOND_METHOD
//        coeff2[file_i] = coeff[file_i];
//#endif
    }
    if(!readable_format)
    {
        update_linef("Converted    ");
        unsigned cc = 1;
        while(cc <= files.size())
        {
            printf("  ");
            unsigned tmp_cc = cc * 10;
            if(tmp_cc < cc)
                break;
            cc = tmp_cc;
        }
        putchar('\n');
        fflush(stdout);
    }
    delete[] pixels;
    delete[] pixels_diff;
    pthread_t show = 0;
    if(show_on)
        pthread_create(&show, &attr, pictures_show, nullptr);

    int range_j = 0;
    unsigned long long compared = 0;

    double fns = 100000, fne = 0, ffs = 100000, ffe = 0, fnmax = 0, famax = 0;
#ifdef SECOND_METHOD
    double sns = 100000, sne = 0, sfs = 100000, sfe = 0, snmax = 0, samax = 0;
    pixels = new unsigned char[wh * wh];
    pixels_diff = new unsigned char[wh * wh];
#endif

    double max_cutoff = 320;
    double cutoff = max_cutoff * (100 - sensibility) / 100;
    for(unsigned i = 0;i < files.size() - 1;i++)
    {
        if(unique && i == unique_ranges[range_j])
            if(++range_j == range_i)
                break;
        if(!(file_type(files[i].path)))
        {
            if(unique)
                compared += unique_ranges[range_i - 1] - unique_ranges[range_j];
            else
                compared += files.size() - i - 1;
            continue;
        }
        unsigned compare_from;
        if(unique)
            compare_from = unique_ranges[range_j];
        else
            compare_from = i + 1;
        Image i1, ih, iv, im;
        i1.read(tmp_dir + files[i].name + dot + files[i].inode + ext);
        if(symmetry)
        {
            if(symmetry_h)
            {
                ih = i1, ih.flop();
                if(symmetry_v)
                    iv = i1, iv.flip(), im = iv, im.flop();
            }
            else
                iv = i1, iv.flip();
        }
        for(unsigned k = compare_from;k < files.size();k++)
        {
            if(!readable_format)
                update_linef("Comparing: %llu%% (%llu/%llu)", compared * 100 / comparisons, compared, comparisons);
            compared++;
            if(!file_type(files[k].path))
                continue;
            if(!method_efficiency && abs(coeff[i] - coeff[k]) * 1048576 / (coeff[i] * coeff[k]) > 18) //max = 13.22 in tests
                continue;
            elif(method_efficiency)
            {
                if(files[i].name[3] == files[k].name[3])
                    set_max(fnmax, abs(coeff[i] - coeff[k]) * 1048576 / (coeff[i] * coeff[k]));
                set_max(famax, abs(coeff[i] - coeff[k]) * 1048576 / (coeff[i] * coeff[k]));
 #ifdef SECOND_METHOD
                if(files[i].name[3] == files[k].name[3])
                    set_max(snmax, abs(coeff2[i] - coeff2[k]) * 1048576 / (coeff2[i] * coeff2[k]));
                set_max(samax, abs(coeff2[i] - coeff2[k]) * 1048576 / (coeff2[i] * coeff2[k]));
 #endif
            }
            Image i2;
            i2.read(tmp_dir + files[k].name + dot + files[k].inode + ext);
            double diff = i1.compare(i2, RootMeanSquaredErrorMetric) * (coeff[i] + coeff[k]);
            if(symmetry)
            {
                if(symmetry_h)
                {
                    set_min(diff, ih.compare(i2, RootMeanSquaredErrorMetric) * (coeff[i] + coeff[k]));
                    if(symmetry_v)
                        set_min(diff, im.compare(i2, RootMeanSquaredErrorMetric) * (coeff[i] + coeff[k]));
                }
                else
                    set_min(diff, iv.compare(i2, RootMeanSquaredErrorMetric) * (coeff[i] + coeff[k]));
            }
#ifdef SECOND_METHOD
            i1.getPixels(0, 0, g1.width(), g1.height());
            i1.writePixels(GrayQuantum, pixels);
            i2.getPixels(0, 0, g1.width(), g1.height());
            i2.writePixels(GrayQuantum, pixels_diff);

            double diff2 = 0.0;
            uint32_t sim = 0, dif = 0;

            for(unsigned wh = 0;wh < g1.height() * g1.width(); wh++)
            {
                unsigned short sim_1 = static_cast<unsigned short>(abs(
                        static_cast<short>(pixels[wh]) - static_cast<short>(pixels_diff[wh])));
                diff2 += static_cast<unsigned short>(pixels[wh]) * static_cast<unsigned short>(pixels_diff[wh]) / (256 - sim_1);
                dif += (static_cast<unsigned short>(static_cast<unsigned short>(pixels[wh]) + static_cast<unsigned short>(pixels_diff[wh])) >> 1u) * sim_1;
            }

            diff2 = (diff2 < 1) ? 100000000 : 1000000 / diff2;

            //double diff2 = sim;
#endif
            if(method_efficiency || diff < cutoff) //diff < 225 for near and > 215 for far in tests
            {
                unsigned char p_diff = static_cast<unsigned char>(100) - static_cast<unsigned char>(round(diff * 100 / max_cutoff));
                if(method_efficiency)
                {
#ifdef SECOND_METHOD
                    printf("\r%s (%f:%f) %s                  \n", files[i].name.c_str(), diff, diff2, files[k].name.c_str());
                    if(files[i].name[3] == files[k].name[3])
                    {
                        set_min(sns, diff2);
                        set_max(sne, diff2);
                    }
                    else
                    {
                        set_min(sfs, diff2);
                        set_max(sfe, diff2);
                    }
#else
                    printf("\r%s (%f) %s                  \n", files[i].name.c_str(), diff, files[k].name.c_str());
#endif
                    if(files[i].name[3] == files[k].name[3])
                    {
                        set_min(fns, diff);
                        set_max(fne, diff);
                    }
                    else
                    {
                        set_min(ffs, diff);
                        set_max(ffe, diff);
                    }
                }
                else
                {
                    if(readable_format)
                        printf("%s\n%hhu\n%s\n", files[i].path.c_str(), p_diff, files[k].path.c_str());
                    else
                        printf("\r%s %s=(%hhu%%)=%s %s                  \n", files[i].path.c_str(), diff == 0 ? "=" : "~", p_diff,
                           diff == 0 ? "=" : "~", files[k].path.c_str());
                }
                fflush(stdout);
                if(show_on)
                {
                    std::unique_lock<std::mutex> s_lock(show_lock);
                    elements_amount++;
                    to_show.push(std::tuple<cstr, cstr, unsigned char>(files[i].path, files[k].path, diff == 0 ? 101 : p_diff));
                    s_lock.unlock();
                    show_cond.notify_one();
                }
            }
        }
    }
    if(!readable_format)
        update_linef("Compared (%llu comparisons)                 \n", compared);
    delete[] coeff;
    delete[] unique_ranges;
    if(method_efficiency)
    {
        printf("First: Near(%f-%f), Far(%f-%f), Cutoff(%f <= %f) \n", fns, fne, ffs, ffe, fnmax, famax);
        if(ffs <= fne)
            printf("Not efficient!(-%f)\n", (fne - ffs) / fne);
        else
            printf("Efficiency: %f\n", (ffs - fne) / fne);
#ifdef SECOND_METHOD
        delete[] coeff2;
        printf("Second: Near(%f-%f), Far(%f-%f), Cutoff(%f <= %f)\n", sns, sne, sfs, sfe, snmax, samax);
        if(sfs <= sne)
            printf("Not efficient!(-%f)\n", (sne - sfs) / sne);
        else
            printf("Efficiency: %f\n", (sfs - sne) / sne);
#endif
    }
    if(show_on)
    {
        std::unique_lock<std::mutex> s_lock(show_lock);
        to_show.push(std::make_tuple(cstr(), cstr(), 0));
        s_lock.unlock();
        show_cond.notify_one();
    }
    if(!readable_format)
        printf("Comparison process ended\n");
    if(show_on)
    {
        if(show_in_background)
            pthread_detach(show);
        else
            pthread_join(show, nullptr);
    }
    return 0;
}
