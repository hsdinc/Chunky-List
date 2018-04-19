/*********************************************************************
 * ChunkyList class.
 *********************************************************************
 *
 * Implementation for the templated ChunkyList
 *
 */
using namespace std;
#include <iostream>
#include <stdexcept>
#include <algorithm>

//constructor 
template<typename ELEMENT>
 ChunkyList<ELEMENT>::ChunkyList():
       size_(0) 
{

}

//copy constructor 
template <typename ELEMENT>
ChunkyList<ELEMENT>::ChunkyList(const ChunkyList& stringToBeCopied):
        size_{stringToBeCopied.size_}, 
        chunks_{stringToBeCopied.chunks_}
{

}

template <typename ELEMENT> 
typename ChunkyList<ELEMENT>::const_iterator ChunkyList<ELEMENT>::begin() const
{
    return const_iterator(0, chunks_.begin());
     //access only --> return a const iterator to the first chunk 
}

template <typename ELEMENT> 
typename ChunkyList<ELEMENT>::iterator ChunkyList<ELEMENT>::begin()
{
    return iterator(0, chunks_.begin());
}

template <typename ELEMENT> 
typename ChunkyList<ELEMENT>::const_iterator ChunkyList<ELEMENT>::end() const
{
    return const_iterator(0, chunks_.end()); 
}

template <typename ELEMENT> 
typename ChunkyList<ELEMENT>::iterator ChunkyList<ELEMENT>::end() 
{
    return iterator(0, chunks_.end());
}

template <typename ELEMENT> 
bool ChunkyList<ELEMENT>::operator==(const ChunkyList<ELEMENT>& rhs) const
{
    if(this->size_ != rhs.size_){
        return false;
    }
    else{
        this->begin();
        const_iterator rhsIt = rhs.begin();
        for(const_iterator i = this->begin(); i != this->end(); ++i){
            if (*i != *rhsIt){
                return false;
            }
            ++rhsIt;
        }
        return true;

    }
}

template <typename ELEMENT> 
bool ChunkyList<ELEMENT>::operator!=(const ChunkyList<ELEMENT>& rhs) const
{
    if(this->size_ != rhs.size_){
        return true;
    }
    else{
        this->begin();
        const_iterator rhsIt = rhs.begin();
        for(const_iterator i = this->begin(); i != this->end(); ++i){
            if (*i != *rhsIt){
                return true;
            }
            ++rhsIt;
        }
        return false;

    }
}

template <typename ELEMENT> 
bool ChunkyList<ELEMENT>::operator<(const ChunkyList<ELEMENT>& rhs) const
{   
    return std::lexicographical_compare(this->begin(), this->end(), 
                                            rhs.begin(), rhs.end());
}

template<typename ELEMENT>
std::ostream& ChunkyList<ELEMENT>::print(std::ostream& out) const
{
    const_iterator i = begin(); 
    while(i!= end()){
        out << *i;
        ++i;
    }
    out <<endl;
    return out;
}

template <typename ELEMENT> 
void ChunkyList<ELEMENT>::push_back(ELEMENT e)
{
    Chunk& lastChunk = chunks_.back();

    if(size_==0 || lastChunk.length_ == Chunk::CHUNKSIZE){
        Chunk newChunk;
        newChunk.elements_[0] = e;
        newChunk.length_ = 1;
        chunks_.push_back(newChunk);
    }
    else{
        lastChunk.elements_[lastChunk.length_] = e;
        lastChunk.length_++; 
    }
    size_++;
}

//helper function that copies the second half to a new chunk 
template <typename ELEMENT>
typename ChunkyList<ELEMENT>::iterator ChunkyList<ELEMENT>::copyUtilization(typename ChunkyList<ELEMENT>::iterator& i)
{
    //create a new chunk
    size_t iteratorPosition = i.arrayIndex_;
    Chunk newChunk;
    newChunk.length_ = 0;
    size_t newChunkIndex = 0;

    //run through this for loop so that the ultization
    // is checked. starting at chunksize/2 and going 
    //all the way to chunksize 
    for(size_t copier = ChunkyList<ELEMENT>::Chunk::CHUNKSIZE/2; 
               copier < ChunkyList<ELEMENT>::Chunk::CHUNKSIZE;
               ++copier){
        //copy over the elements into the new chunk by 
        //dereferencing itUt_ 
        ++newChunk.length_;
        --(i.it_->length_);
        newChunk.elements_[newChunkIndex] = (*i.it_).elements_[copier];
        //update the lengths to accomodate the new chunk
        ++newChunkIndex;
        }
   
    //if our iterator wasn't in the first half, point it to wherever it is
    if (iteratorPosition >= ChunkyList<ELEMENT>::Chunk::CHUNKSIZE/2){ 
        iterator passIt {(iteratorPosition - ChunkyList<ELEMENT>::Chunk::CHUNKSIZE/2),
                         chunks_.insert(++i.it_,newChunk)};          
        return passIt;
        
    }
    else{
        chunks_.insert(i.it_,newChunk);
        iterator passIt {iteratorPosition, i.it_};
        return passIt;
    }
}


//helper function to add the element to the wanted space
template <typename ELEMENT> 
void ChunkyList<ELEMENT>::insertToChunk(typename ChunkyList<ELEMENT>::iterator i, ELEMENT c){
    size_t insertLocation = i.arrayIndex_;

    //moves all the elements 1 forward to create space for the pushee
    for(int mover = i.it_->length_; mover >= insertLocation ; --mover){
        i.it_->elements_[mover] = i.it_->elements_[mover-1];
    }
    //inserts to wanted space
    i.it_->elements_[insertLocation] = c;
    ++size_;
    ++(i.it_->length_);
}

template <typename ELEMENT> 
typename ChunkyList<ELEMENT>::iterator ChunkyList<ELEMENT>::insert(ELEMENT c, iterator i)
{
    //if we are inserting at the end of the list and it is not full, just push_back;
    if(i.it_->length_ != ChunkyList<ELEMENT>::Chunk::CHUNKSIZE && i.arrayIndex_ == i.it_->length_ -1 ){
        push_back(c);
        return i;
    }
    //if we are adding to a full chunk, call copyUtilization and then insertToChunk.
    else if(i.it_->length_ == ChunkyList<ELEMENT>::Chunk::CHUNKSIZE){   
        insertToChunk(copyUtilization(i),c);
        return i;
    }   
    //if the chunk is not full, just call insertToChunk
    else if(i.it_->length_ != ChunkyList<ELEMENT>::Chunk::CHUNKSIZE){
        insertToChunk(i,c);
        print(cout);
        return i;
    }
    
}
//////////////////////////////////
//ERASE FUNCTION//////////////////
//////////////////////////////////

template <typename ELEMENT> 
typename ChunkyList<ELEMENT>::iterator ChunkyList<ELEMENT>::erase(iterator i)
{
    //if the chunkyList has less than 3 elements or chunk has enough elements just delete 
    if(size_ <=3 || i.it_->length_ > 3){
        return deleteFromChunk(i);

    }
    //if we need utilization 
    else if (i.it_->length_ =3){
        iterator it = deleteFromChunk(it);
        return copyNewUtilization(it);
    }
}

//delete from chunk helper function
template <typename ELEMENT>
typename ChunkyList<ELEMENT>::iterator 
ChunkyList<ELEMENT>::deleteFromChunk(typename ChunkyList<ELEMENT>::iterator& i){
    //create a variable for where you want to delete
    size_t deleteLocation = i.arrayIndex_;
    for(size_t remover = deleteLocation; remover < i.it_->length_ ; ++remover){
        i.it_->elements_[remover] = i.it_->elements_[remover + 1];
    }
    //update size
    --size_;
    //update length
    --(i.it_->length_);
    return i;
    
}

//utilization helper function for erase
template <typename ELEMENT>
typename ChunkyList<ELEMENT>::iterator 
ChunkyList<ELEMENT>::copyNewUtilization(typename ChunkyList<ELEMENT>::iterator& i)
{   
    //if((*i.it))
    if ( i.it_ !=  --chunks_.end() ){
        return util(i);
    }
    else{
        return utilLast(i);
    }
}

template <typename ELEMENT>
typename ChunkyList<ELEMENT>::iterator 
ChunkyList<ELEMENT>::utilLast(typename ChunkyList<ELEMENT>::iterator& i){
    chunk_iter_t previousChunk = --(i.it_);
    if (previousChunk->length_ > 9){
        ELEMENT elementToCopy = previousChunk->elements_[previousChunk->length_-1];
        --previousChunk->length_;
        ++(*i.it_).length_;
        for (size_t  mover = (*i.it_)->length_-1; mover >= 0; --mover){
            (*i.it_).elements_[mover] = previousChunk->elements_[mover+1];
        }
        ++(*i.it_)->length_;
        (*i.it_).elements_[0] = elementToCopy;
        return i;
    } 
    else{
        for (size_t mover = 0; mover < (*i.it_)->length_; ++mover){
            previousChunk->elements_[previousChunk->length_] = (*i.it).elements_[mover];
            ++previousChunk->length_;       
        }
        erase(i.it_);
        return i;
    } 
 }


template <typename ELEMENT>
typename ChunkyList<ELEMENT>::iterator 
ChunkyList<ELEMENT>::util(typename ChunkyList<ELEMENT>::iterator& i){
    const_iterator nextChunk = ++(i.it_);
    //if length_ >9
    if (nextChunk->length_ > 9){
        ELEMENT elementToCopy = nextChunk->elements_[0];
        (*i.it_).elements_[(*i.it_).length_] = elementToCopy;
        ++(*i.it_).length_;
        for (size_t mover = 0; mover < nextChunk->length_; ++mover){
            (*i.it_).elements_[mover] = nextChunk->elements_[mover+1];       
        }
        --nextChunk->length_;
        return i;
    }
    else{
        size_t toMove = (*i.it_).length_;
        for (size_t  mover = nextChunk->length_-1; mover >= 0; --mover){
            nextChunk->elements_[mover+toMove] = nextChunk->elements_[mover];
        } 
        //increase the length of the adjacent chunk and then delete the previous chunk
        nextChunk->length_ = nextChunk->length_ + toMove;
        erase(i.it_);
        return i;
    }
 }



//////////
////////


template <typename ELEMENT> 
ChunkyList<ELEMENT>& ChunkyList<ELEMENT>::operator+=(const ChunkyList<ELEMENT>& rhs)
{
    if(*this != rhs){
        for(const_iterator rhsIt = rhs.begin(); rhsIt != rhs.end(); ++rhsIt){
            push_back(*rhsIt);
        }
    }
    else{
        ChunkyList<ELEMENT> copied = ChunkyList<ELEMENT>(*this);
        for(const_iterator rhsIt = copied.begin(); rhsIt != copied.end(); ++rhsIt){
            push_back(*rhsIt);
        }    
    }
    return *this;
}

template <typename ELEMENT> 
size_t ChunkyList<ELEMENT>::size() const{
    return ChunkyList<ELEMENT>::size_;
}

template <typename ELEMENT> 
double ChunkyList<ELEMENT>::utilization() const{
    double maxElement = Chunk::CHUNKSIZE*chunks_.size();
    double currentElement = ChunkyList<ELEMENT>::size_;
    return currentElement/maxElement;
}



/*


template <typename ELEMENT> 
template<bool is_const>
typename ChunkyList<ELEMENT>::iterator erase(iterator i)
{
    throw std::logic_error("Not yet implemented");
}
*/




/////////////////////////////
///////Iterator Class////////
/////////////////////////////


template <typename ELEMENT> 
template<bool is_const> 
ChunkyList<ELEMENT>::Iterator<is_const>::Iterator(size_t arrayIndex, chunk_iter_t it):
    arrayIndex_{arrayIndex},
    it_{it}
{
    
}

template <typename ELEMENT> 
template<bool is_const>
ChunkyList<ELEMENT>::Iterator<is_const>::Iterator(const Iterator<false>& i):
    arrayIndex_{i.arrayIndex_},
    it_{i.it_}
{

}

template <typename ELEMENT> 
template <bool is_const>
ChunkyList<ELEMENT>::Iterator<is_const>& ChunkyList<ELEMENT>::Iterator<is_const>::operator++()
{

    ++arrayIndex_;
    if(arrayIndex_ == it_->length_){
        ++it_;
        arrayIndex_ = 0;
        
    }
    return *this;
}

template <typename ELEMENT> 
template<bool is_const>
ChunkyList<ELEMENT>::Iterator<is_const>& ChunkyList<ELEMENT>::Iterator<is_const>::operator--()
{
    if(arrayIndex_ != 0){
        --arrayIndex_;
        return *this;
    }
    else{
        --it_;
        arrayIndex_ = it_->length_ - 1; 
        return *this;
    }
}

template <typename ELEMENT> 
template<bool is_const>
typename ChunkyList<ELEMENT>::template Iterator<is_const>::reference 
    ChunkyList<ELEMENT>::Iterator<is_const>::operator*() const
{

    return it_->elements_[arrayIndex_];
    
}

template <typename ELEMENT> 
template<bool is_const>
bool ChunkyList<ELEMENT>::Iterator<is_const>::operator==(const Iterator& rhs) const
{
    if (this->it_ == rhs.it_){
        if (this->arrayIndex_ == rhs.arrayIndex_){
            return true;
        }
        else{
            return false;
        }
    }
    else{
        return false;
    }
}

template <typename ELEMENT> 
template<bool is_const>
bool ChunkyList<ELEMENT>::Iterator<is_const>::operator!=(const Iterator& rhs) const
{
   return !(*this == (rhs));
}

