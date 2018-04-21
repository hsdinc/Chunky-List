    /**
     * \class Iterator
     * \brief STL-style iterator for ChunkyString.
     *
     * \details Synthesized copy constructor, destructor, and assignment
     *          operator are okay.
     *
     *          The five typedefs and the member functions are such that
     *          the iterator works properly with STL functions (e.g., copy).
     *
     *          Since this is a bidirectional_iterator, `operator--`
     *          is provided and meaningful for all iterators except
     *          ChunkyString::begin.
     *
     *  \remarks The design of the templated iterator was inspired by these
     *           two sources:
     *  www.drdobbs.com/the-standard-librarian-defining-iterato/184401331
     *  www.sj-vs.net/c-implementing-const_iterator-
     * and-non-const-iterator-without-code-duplication
     */
#include <list>
template <typename ELEMENT>
class ChunkyList {
private:
    // Forward declaration of Iterator
    template <bool is_const>
    class Iterator;

public:
    ChunkyList();
    //copy constructor
    ChunkyList(const ChunkyList& stringToBeCopied);
    // Standard STL container type definitions
    using value_type      = ELEMENT;
    using size_type       = size_t;
    using difference_type = ptrdiff_t;
    using reference       = value_type&;
    using const_reference = const value_type&;
    using iterator        = Iterator<false>;
    using const_iterator  = Iterator<true>;
    // reverse_iterator and const_reverse_iterator aren't supported 
    



    //ChunkyList Function
    const_iterator end() const;
    const_iterator begin() const; 
    iterator end();
    iterator begin();
    std::ostream& print(std::ostream& out) const;
    void push_back(ELEMENT e);
    size_t size() const;
    ChunkyList<ELEMENT>& operator+=(const ChunkyList<ELEMENT>& rhs);
    bool operator==(const ChunkyList<ELEMENT>& rhs) const;
    bool operator!=(const ChunkyList<ELEMENT>& rhs) const;
    bool operator<(const ChunkyList<ELEMENT>& rhs) const;
    //make sure to add insert and erase next week
    double utilization() const;
    //HAZAL
    void insertToChunk(iterator i, ELEMENT elementToInsert);
    typename ChunkyList<ELEMENT>::iterator copyUtilization(iterator& i);
    typename ChunkyList<ELEMENT>::iterator insert(ELEMENT c, iterator i);
    typename ChunkyList<ELEMENT>::iterator erase(iterator i);
    typename ChunkyList<ELEMENT>::iterator utilLast(iterator& i);
    typename ChunkyList<ELEMENT>::iterator util(iterator& i);
    typename ChunkyList<ELEMENT>::iterator copyNewUtilization(iterator& i);
    typename ChunkyList<ELEMENT>::iterator 
             deleteFromChunk(typename ChunkyList<ELEMENT>::iterator& i);
   


    


private: 

    struct Chunk{
        static const size_t CHUNKSIZE = 12;
        size_t length_;
        ELEMENT elements_[CHUNKSIZE];
    };

    size_t size_;
    std::list<Chunk> chunks_;
    
    template <bool is_const>
    class Iterator {
    public:
        


        // Make Iterator STL-friendly with these typedefs:
        using difference_type   = ptrdiff_t;
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type        = ELEMENT;
        using const_reference   = const value_type&;
        
        using reference = typename std::conditional<is_const, 
        const value_type&, value_type&>::type;
        
        using pointer = typename std::conditional<is_const,
        const value_type*, value_type*>::type;

        /**
        *  The `chunk_iter_t` type below might be helpful for some 
        *  implementations. If your implementation ever needs to declare
        *  a variable whose type is an iterator for a chunk list, you 
        *  should use `chunk_iter_t` for the type name. 
        */
        using chunk_iter_t = typename std::conditional<is_const, 
                            typename std::list<Chunk>::const_iterator, 
                            typename std::list<Chunk>::iterator>::type;
    
        ///<  constructor
        Iterator(size_t arrayIndex, chunk_iter_t it);

        ///< Convert a non-const iterator to a const-iterator, if necessary
        Iterator(const Iterator<false>& i);  

        //Iterator Operations
        Iterator& operator++();
        Iterator& operator--();
        reference operator*() const;
        bool operator==(const Iterator& rhs) const;
        bool operator!=(const Iterator& rhs) const;

    private:
        friend class ChunkyList<ELEMENT>;
        size_t arrayIndex_;
        chunk_iter_t it_;

    };
};

#include "chunkylist-private.hpp"

    


   
