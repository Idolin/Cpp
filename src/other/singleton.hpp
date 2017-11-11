template<class S>
class singleton
{
    S s;

    singleton &operator=(singleton const &) = delete;

    singleton(singleton const &) = delete;

public:
    static S &instance()
    {
        static singleton s;
        return s.s;
    }

private:
    singleton()
    {}

    ~singleton()
    {}
};
