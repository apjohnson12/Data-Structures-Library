#include <iostream>

using namespace std;

template <class elmtype>

class CircularDynamicArray
{
private:

  elmtype *_array;
  elmtype BAD;
  int _capacity;
  int _start;
  int _end;

  /*void _grow(){
    elmtype *temp = _array;
    _array = new elmtype[_capacity*2];
    int end = 0;
    for (int i=_start; i<_end; i++){
      _array[end++] = temp[i%_capacity];
    }
    _start = 0;
    _end = end;
    _capacity *= 2;
    //cout << "capacity is " << _capacity << endl;
    //cout << temp << endl;
    delete[] temp;
  }*/

  /*void _shrink(){
    if (_capacity == 2){
      return;
    }
    elmtype *temp = _array;
    _array = new elmtype[_capacity/2];
    int end = 0;
    for (int i=_start; i<_end; i++){
      _array[end++] = temp[i%_capacity];
    }
    _start = 0;
    _end = end;
    _capacity /= 2;
    delete[] temp;
  }*/
  elmtype _kSmallest(int l, int r, int k){
    //cout << l << " " << r << " " << k << endl;
    if (k > 0 && k <= r-l+1){
      int index = _partition(l, r);
      //cout << "index after partition is " << index << endl;
      if (index - l == k - 1){
        //cout << "final index is : " << index << endl;
        return (*this)[index];
      }
      if (index - l > k - 1){
        return _kSmallest(l, index-1, k);
      }
      return _kSmallest(index + 1, r, k-index+l-1);
    }
    return -1;
  }

  int _partition(int left, int right){
    int x = (*this)[right];
    int i = left;

    for (int j = left; j<=right-1; j++){
      if ((*this)[j]<=x){
        elmtype temp = (*this)[i];
        (*this)[i] = (*this)[j];
        (*this)[j] = temp;
        i++;
      }
    }
    elmtype temp = (*this)[i];
    (*this)[i] = (*this)[right];
    (*this)[right] = temp;
    return i;
  }

  void _swap(int left, int right){
    elmtype temp = (*this)[left];
    (*this)[left] = (*this)[right];
    (*this)[right] = temp;
  }

  int _partition5(int left, int right){
    int i = left + 1;
    while (i <= right){
      int j = i;
      while (j > left && (*this)[j-1] > (*this)[j]){
        _swap(j-1, j);
        j--;
      }
      i++;
    }
    return (left + right)/2;
  }

  int _pivot(int left, int right){
    if (right - left < 5){
      return _partition5(left, right);
    }
    for (int i = left; i <= right; i+=5){
      int subRight = i + 4;
      if (subRight > right){
        subRight = right;
      }
      int median5 = _partition5(i, subRight);
      _swap(median5, left + ((i-left)/5));
    }
    int mid = (right - left)/10 + left + 1;
    return _select(left, left + (right - left)/5, mid);
  }

  int _select(int left, int right, int n){
    while (true){
      if (left == right){
        return left;
      }
      int pivotIndex = _pivot(left, right);
      pivotIndex = _partition(left, right, pivotIndex, n);
      if (n == pivotIndex){
        return n;
      }
      else if (n < pivotIndex){
        right = pivotIndex - 1;
      }
      else {
        left = pivotIndex + 1;
      }
    }
  }

  int _partition(int left, int right, int pivotIndex, int n){
    elmtype pivotValue = (*this)[pivotIndex];
    _swap(pivotIndex, right);
    int storeIndex = left;

    for (int i = left; i < right; i++){
      if ((*this)[i] < pivotValue){
        _swap(storeIndex, i);
        storeIndex++;
      }
    }
    int storeIndexEq = storeIndex;
    for (int i = storeIndex; i < right; i++){
      if ((*this)[i] == pivotValue){
        _swap(storeIndexEq, i);
        storeIndexEq++;
      }
    }
    _swap(right, storeIndexEq);

    if (n < storeIndex){
      return storeIndex;
    }
    if (n < storeIndexEq){
      return n;
    }
    return storeIndexEq;
  }

  elmtype* _resize(elmtype* array, int size) {
    elmtype *temp = new elmtype[size];
    for (int i = 0; i < length(); i++){
      temp[i] = array[(i+_start+_capacity)%_capacity];
    }
    return temp;
    //_end = length();
    //_start = 0;
    //delete[] _array;
    //_array = temp;
    //_capacity = size;
  }

public:

  void _grow(){
    elmtype *temp = _array;
    _array = new elmtype[_capacity*2];
    int end = 0;
    for (int i=_start; i<_end; i++){
      _array[end++] = temp[(i+_capacity)%_capacity];
    }
    _start = 0;
    _end = end;
    _capacity *= 2;
    //cout << "capacity is " << _capacity << endl;
    //cout << temp << endl;
    delete[] temp;
  }

  void _shrink(){
    if (_capacity == 2){
      return;
    }
    elmtype *temp = _array;
    _array = new elmtype[_capacity/2];
    int end = 0;
    for (int i=_start; i<_end; i++){
      _array[end++] = temp[(i+_capacity)%_capacity];
    }
    _start = 0;
    _end = end;
    _capacity /= 2;
    delete[] temp;
  }

  CircularDynamicArray()
  {
    _capacity = 2;
    _array = new elmtype[_capacity];
    _start = 0;
    _end = 0;
  }

  CircularDynamicArray(int s)
  {
    _capacity = s;
    _array = new elmtype[_capacity];
    _start = 0;
    _end = s;
  }

  CircularDynamicArray(const CircularDynamicArray<elmtype> &original){
    _capacity = original.capacity();
    _array = new elmtype[_capacity];
    _start = 0;
    _end = original.length();
    for (int i = 0; i < _end; i++){
      (*this)[i] = original[i];
    }
  }

  CircularDynamicArray& operator=(const CircularDynamicArray<elmtype> &original){
    delete[] _array;
    _capacity = original._capacity;
    _array = new elmtype[original._capacity];
    _start = 0;
    _end = original._end - original._start;
    for (int i = 0; i < _end; i++){
      (*this)[i] = original._array[(original._start+i+_capacity)%_capacity];
    }
    return *this;
  }

  ~CircularDynamicArray()
  {
    delete[] _array;
    //if (_array != nullptr){
      //delete[] _array;
    //}
    //_array = nullptr;
  }



  elmtype& operator[](int i)
  {
    if (i<length() && i>-1){
      //cout << "Index is out of bounds. Please try another" << endl;
      return _array[(_start+i+_capacity)%_capacity];
    }
    else {
      cout << "Out of bounds reference : " << i << endl;
      return this->BAD;
    }


  }

  void addEnd(elmtype v)
  {
    if (length() == _capacity){
      _grow();
      /*elmtype* temp = _resize(_array, capacity()*2);
      delete[] _array;
      _array = temp;
      _end = length();
      _start = 0;
      _capacity *= 2;*/

    }
    _array[_end%_capacity] = v;
    _end++;
    if (_end>=_capacity && _start>=_capacity){
      _end-=_capacity;
      _start-=_capacity;
    }
  }

  void addFront(elmtype v){
    if (length() == _capacity){
      //cout << _start << " " << _end << " " << _capacity << endl;
      _grow();
      /*elmtype* temp = _resize(_array, capacity()*2);
      delete[] _array;
      _array = temp;
      _end = length();
      _start = 0;
      _capacity *= 2;*/
    }
    _start--;

    _array[(_start+_capacity)%_capacity] = v;
    if (_end<0 && _start<0) {
      _end+=_capacity;
      _start+=_capacity;
    }
  }

  int length(){
    return _end-_start;
  }

  void delEnd(){
    if (length() > 0){
      _end--;
      if (_end < 0 && _start < 0){
        _end += _capacity;
        _start += _capacity;
      }
    }
    if (_capacity/length() >= 4){
      _shrink();
      /*elmtype* temp = _resize(_array, capacity()/2);
      delete[] _array;
      _array = temp;
      _end = length();
      _start = 0;
      _capacity /= 2;*/
    }
  }

  void delFront(){
    if (length() > 0){
      _start++;
      if (_end >= _capacity && _start >= _capacity){
        _end -= _capacity;
        _start -= _capacity;
      }
    }
    if (_capacity/length() >= 4){
      _shrink();
      /*elmtype* temp = _resize(_array, capacity()/2);
      delete[] _array;
      _array = temp;
      _end = length();
      _start = 0;
      _capacity /= 2;*/
    }
  }

  int capacity(){
    return _capacity;
  }

  void clear(){
    delete[] _array;
    _capacity = 2;
    _array = new elmtype[_capacity];
    _start = 0;
    _end = 0;
  }

  elmtype QuickSelect(int k){
    CircularDynamicArray<elmtype> work(length());
    for (int i = 0; i < length(); i++){
      work[i] = (*this)[i];
    }
    int left = 0;
    int right = length()-1;
    return work._kSmallest(left, right, k);
  }

  elmtype WCSelect(int k){
    CircularDynamicArray<elmtype> work(length());
    for (int i = 0; i < length(); i++){
      work[i] = (*this)[i];
    }
    int index = work._select(0, length()-1, k-1);
    return work[index];
  }

  void stableSort(){
    if (length() == 1){
      return;
    }
    int leftSize = length()/2;
    int rightSize = length()-leftSize;
    CircularDynamicArray<elmtype> left(leftSize);
    CircularDynamicArray<elmtype> right(rightSize);

    int i;
    for (i = 0; i < leftSize; i++){
      left[i] = (*this)[i];
    }
    for (int j = 0; j < rightSize; j++){
      right[j] = (*this)[i++];
    }

    left.stableSort();
    right.stableSort();
    int li = 0;
    int ri = 0;
    i = 0;
    while (li < left.length() && ri < right.length()){
      if (left[li] < right[ri]){
        (*this)[i++] = left[li++];
      }
      else {
        (*this)[i++] = right[ri++];
      }
    }

    while (li < left.length()){
      (*this)[i++] = left[li++];
    }
    while (ri < right.length()){
      (*this)[i++] = right[ri++];
    }
  }

  void radixSort(int i){
    elmtype bucketZero[length()];
    elmtype bucketOne[length()];
    int oneEnd;
    int zeroEnd;
    for (int j = 0; j < i; j++){
      oneEnd = 0;
      zeroEnd = 0;
      for (int k = 0; k < length(); k++){
        int bucket = (*this)[k] >> j & 1;
        //cout << "bucket is " << bucket << endl;
        if (bucket == 0){
          bucketZero[zeroEnd++] = (*this)[k];
        }
        else{
          bucketOne[oneEnd++] = (*this)[k];
        }
      }
      int fill = 0;
      for (int k = 0; k < zeroEnd; k++){
        (*this)[fill++] = bucketZero[k];
      }
      for (int k = 0; k < oneEnd; k++){
        (*this)[fill++] = bucketOne[k];
      }
    }
  }

  int linearSearch(elmtype e){
    for (int i = 0; i<this->length(); i++){
      if ((*this)[i] == e){
        return i;
      }
    }
    return -1;
  }

  int binSearch(elmtype e) {
    int start = 0;
    int end = length();
    int mid;

    while (end > start) {
      mid = (start+end)/2;
      if ((*this)[mid] == e){
        return mid;
      }
      else if ((*this)[mid] > e){
        end = mid;
      }

      else {
        start = mid+1;
      }
    }
    return -1;
  }

};
