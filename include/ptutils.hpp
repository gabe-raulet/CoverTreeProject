template <class PointTraits, class Index>
void PointUtils<PointTraits, Index>::read_points_file(PointVector& points, const char *fname, bool verbose)
{
    LocalTimer timer;
    timer.start_timer();

    points.clear();
    PointTraits::read_from_file(back_inserter(points), fname);

    timer.stop_timer();

    fprintf(stderr, "[time=%.3f,msg::%s] read points file '%s' [numpts=%lu,filesize=%s]\n", timer.get_elapsed(), __func__, fname, static_cast<size_t>(points.size()), FileInfo(fname).get_file_size_str());
}
