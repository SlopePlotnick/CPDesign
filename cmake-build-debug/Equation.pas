program PrimTest;
    var answer, inputa, inputb, inputc;
    procedure hasAnswer(input1, input2, input3);
        var D;
        procedure delta(a, b, c);
            begin
                D := b * b - 4 * a * c;
                write(D);
                if D >= 0 then
                    answer := 1
                else
                    answer := 0
            end
        begin
            call delta(input1, input2, input3);
            write(D)
        end
    begin
        read(inputa, inputb, inputc);
        call hasAnswer(inputa, inputb, inputc);
        write(answer)
    end